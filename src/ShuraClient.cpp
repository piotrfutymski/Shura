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
        engine.sigInt();
        isRunning=false;
        std::cout << "Disconnected" << std::endl;
        });

    while(!serverStarted && isRunning){
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
        }
    if(isRunning)
        engine.run();
    isRunning = false;
    serverUpdateThread->join();  
    delete serverUpdateThread;
    
    shutdown(sd, SHUT_RDWR);
    close(sd);
}

void ShuraClient::serverBinding()
{  
    nlohmann::json msg, prevMsg; 
    do
    {
        msg = Network::receiveMsg(sd);
        if(msg["start"] == true)
            break;
        if(prevMsg != msg)
        {
            std::cout << "##########\n";
            std::cout << "Players:\n";
            for(nlohmann::json::iterator it = msg["players"].begin(); it != msg["players"].end(); it++)
                std::cout << (*it) << "\n";
            std::cout << "##########" << std::endl;
        }
        prevMsg = msg;
        
    }while(isRunning);
    
    serverStarted = true;
    while(isRunning)
    {
       Network::sendMsg(sd, game->getClientJson());
       msg = Network::receiveMsg(sd);
       if(msg["end"] == true)
            break;
            
       game->setServerJson(msg);
    }
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

bool ShuraClient::containsName(const nlohmann::json & players, const std::string & name)
{
    for(nlohmann::json::const_iterator it = players.begin(); it != players.end(); it++)
    {
        if((*it) == name)
            return true;
    }
    return false;
}
void ShuraClient::deleteName(nlohmann::json & players, const std::string & name)
{
    for(nlohmann::json::iterator it = players.begin(); it != players.end(); )
    {
        if((*it) == name)
            players.erase(it);
        else
            it++;       
    }
}