#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <thread>
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include "nlohmann/json.hpp"

class ShuraServer
{
public:
    ShuraServer();
    ShuraServer(ShuraServer&) = delete;
    ShuraServer(ShuraServer&&) = delete;
    ShuraServer& operator=(ShuraServer&) = delete;
    ShuraServer& operator=(ShuraServer&&) = delete;

    void run(const char * portStr);

private:

    sockaddr_in localAddress;
    int sd;
    bool isRunning;
    nlohmann::json game;

    std::vector<std::thread*> clientThreads;
    std::thread* serverThread;

    void clientWork(int fd);
    void serverWork();
};