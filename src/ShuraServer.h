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
#include <fcntl.h>
#include "nlohmann/json.hpp"
#include "Game.h"
#include "Network.h"
#include <condition_variable>
#include "JsonUtil.h"

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
    nlohmann::json gameInfo;
    std::shared_ptr<Game> game;
    std::mutex mut;
    Didax::Engine engine;

    std::vector<std::thread*> clientThreads;
    std::vector<int> freeId;
    std::thread* serverThread;

    void prepareSocket(const char * portStr);

    std::string winner;
    volatile bool clientWorkState;
    volatile bool waitForInitState;
    void clientWork(int fd);

    volatile bool joinWorkState;
    void joinWork();
    std::string clientRegistration(int fd);
};