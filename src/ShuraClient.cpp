#include "ShuraClient.h"


ShuraClient::ShuraClient() : sd(-1), gameinfo() {}

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

    gameinfo["register"] = "Kuba";
    std::string data = gameinfo.dump();
    int len = data.size();
    write(sd, &len, sizeof(int));
    write(sd, data.c_str(), len);
    auto serverUpdateThread = new std::thread([this](){ this->serverBinding(); });
    runGame();
    shutdown(sd, SHUT_RDWR);
    close(sd);
}

void ShuraClient::runGame()
{
    Didax::Engine engine;
    engine.init("data/settings.json");
    game = std::make_shared<Game>();
    engine.addEntity<Didax::Scriptable<Game>>(game, "Game");
    engine.run();
}

void ShuraClient::serverBinding()
{

}