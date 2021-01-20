#include "ShuraServer.h"

ShuraServer::ShuraServer()
    :localAddress({}), sd(-1) {}

void ShuraServer::run()
{
    start();
    
    std::vector<char> data(2048);
    while(true) {
        int dataSize = recv(sd, data.data(), 1, MSG_TRUNC|MSG_PEEK);
        if(dataSize<1) 
            break;
        
        data.resize(dataSize);
        if(recv(sd, data.data(), dataSize, 0) < 1) 
            break;
    }
}

void ShuraServer::start()
{
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(defaultPort);
    localAddress.sin_addr = {htonl(INADDR_ANY)};

    sd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sd == -1)
        throw std::runtime_error("socket error");

    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, NULL, 0) != 0)
        throw std::runtime_error("setsockopt error");
    
    if(bind(sd, (sockaddr*)&localAddress, sizeof(localAddress)) != 0)
        throw std::runtime_error("bind error");
    
}