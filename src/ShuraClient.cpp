#include "ShuraClient.h"


ShuraClient::ShuraClient() : isRunning(false), sd(-1) {}

void ShuraClient::run(const char *ipStr, const char *portStr)
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
    
    std::string name = registerClient();
    isRunning = true;
    auto serverUpdateThread = new std::thread([this](){ this->serverBinding(); });
    runGame(name);
    isRunning = false;
    serverUpdateThread->join();
    delete serverUpdateThread;
    shutdown(sd, SHUT_RDWR);
    close(sd);
}

void ShuraClient::runGame(const std::string & name)
{
    Didax::Engine engine;
    engine.init("data/settings.json");
    game = std::make_shared<Game>(true);
    game->setName(name);
    engine.addEntity<Didax::Scriptable<Game>>(game, "Game");
    game->addPlayer(name);
    engine.run();
}

void ShuraClient::serverBinding()
{
     nlohmann::json msg;
     while(isRunning)
     {
        game->push_Keys(msg);

        std::string data = msg.dump();
        int len = data.size();
        write(sd, &len, sizeof(int));
        write(sd, data.c_str(), len);

        msg.clear();
        recv(sd, &len, sizeof(int), MSG_WAITALL);
        std::vector<char> buf(len);
        recv(sd, buf.data(), len, MSG_WAITALL);
        msg = nlohmann::json::parse(buf.data());
        
        game->actualizeState(msg);
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
        std::string data = tmp.dump();
        int len = data.size();
        write(sd, &len, sizeof(int));
        write(sd, data.c_str(), len);

        nlohmann::json msg;
        int msgSize;
        if(recv(sd, &msgSize, sizeof(int), MSG_WAITALL) == -1)
            throw std::runtime_error("register error");
        std::vector<char> buf(msgSize);
        if(recv(sd, buf.data(), msgSize, MSG_WAITALL) == -1)
            throw std::runtime_error("register error");
        msg = nlohmann::json::parse(buf.data());
        if(msg["priv"]["register"])
            break;   
        std::cout<< "Somebody has this name, choose another: ";   
    }
    return name;
}