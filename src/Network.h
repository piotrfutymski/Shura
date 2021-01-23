#pragma once
#include "nlohmann/json.hpp"
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>

class Network
{
public:
static nlohmann::json receiveMsg(int fd)
{
    nlohmann::json msg;
    int msgSize;
    if(recv(fd, &msgSize, sizeof(int), MSG_WAITALL) < 1)
        throw std::runtime_error("recv error");
    std::vector<char> tmp(msgSize);
    if(recv(fd, tmp.data(), msgSize, MSG_WAITALL) < 1)
        throw std::runtime_error("recv error");
    msg = nlohmann::json::parse(tmp.begin(), tmp.end());
    return msg;
}

static void sendMsg(int fd, const nlohmann::json &msg)
{
    std::string data = msg.dump();
    int len = data.length();
    write(fd, &len, sizeof(int));      
    write(fd, data.c_str(), len);   
}
};
