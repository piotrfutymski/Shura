#include "ShuraClient.h"

ShuraClient::ShuraClient() : sd(-1), game() {}

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

    game["register"] = "Kuba";
    std::string data = game.dump();
    size_t len = data.size();
    write(sd, &len, sizeof(size_t));
    write(sd, data.c_str(), len);

    close(sd);
}