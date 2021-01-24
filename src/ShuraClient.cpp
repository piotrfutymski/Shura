#include "ShuraClient.h"


ShuraClient::ShuraClient() : isRunning(false), sd(-1) {}

void ShuraClient::prepareSocket(const char *ipStr, const char * portStr)
{
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *resolved;
    if(getaddrinfo(ipStr, portStr, &hints, &resolved) != 0)
        throw std::runtime_error("getaddrinfo error");

    sd = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    if(sd == -1)
        throw std::runtime_error("socket error");
    
    if(connect(sd, resolved->ai_addr, resolved->ai_addrlen) != 0)
        throw std::runtime_error("connect error");
    freeaddrinfo(resolved);
}

void ShuraClient::run(const char *ipStr, const char *portStr)
{
    prepareSocket(ipStr, portStr);
    std::string name = registerClient();
   
    engine.init("data/settings.json");
    game = std::make_shared<Game>(true, name, _id);
    engine.addEntity<Didax::Scriptable<Game>>(game, "Game");

    isRunning = true;
    serverStarted = false;
    auto serverUpdateThread = new std::thread([this](){  
        try{
            this->serverBinding();
        }
        catch(const std::exception & e) {;}
        });

    while(!serverStarted){
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);}
    engine.run();
    isRunning = false;
    serverUpdateThread->join();  
    delete serverUpdateThread;
    
    shutdown(sd, SHUT_RDWR);
    close(sd);
}

void ShuraClient::serverBinding()
{   
    //using namespace std::chrono_literals;
    //std::this_thread::sleep_for(50ms);
    nlohmann::json msg; 
    do
    {
        msg = Network::receiveMsg(sd);
        nlohmann::json imPresent;
        imPresent["present"] = true;
        Network::sendMsg(sd, imPresent);
        
    }while(msg["start"] == false && isRunning);
    
    nlohmann::json keys;
    serverStarted = true;
    while(isRunning)
    {
       keys["pls_dont_be_null"] = "ok";
       engine.lock();
       game->push_Keys(keys);
       engine.unlock();

       Network::sendMsg(sd, keys);
       msg = Network::receiveMsg(sd);
       if(msg.contains("end"))
            break;
            
       engine.lock();
       game->actualizeState(msg);
       engine.unlock();
    }

    nlohmann::json imPresent;
        imPresent["present"] = false;
    Network::sendMsg(sd, imPresent);



}

std::string ShuraClient::registerClient()
{
    std::string name;
    std::cout<< "Choose your hero name: ";
    while(true)
    {   
        std::cin >> name;

        nlohmann::json tmp;
        tmp["register"] = name;
        Network::sendMsg(sd, tmp);

        nlohmann::json msg = Network::receiveMsg(sd);
        if(msg["priv"]["register"])
        {
            _id = msg["priv"]["id"]; 
            break;   
        }         
        std::cout<< "Somebody has this name, choose another: ";
    }
    
    return name;
}

