#include "ShuraServer.h"

ShuraServer::ShuraServer()
    :localAddress({}), sd(-1), isRunning(false), game() {}

void ShuraServer::run(const char * portStr)
{
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(atoi(portStr));
    localAddress.sin_addr = {htonl(INADDR_ANY)};

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd == -1)
        throw std::runtime_error("socket error");

    int one = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) != 0)
        throw std::runtime_error("setsockopt error");
    
    if(bind(sd, (sockaddr*)&localAddress, sizeof(localAddress)) != 0)
        throw std::runtime_error("bind error");
    
    if(listen(sd, SOMAXCONN) != 0)
        throw std::runtime_error("listen error");

    isRunning = true;

    std::cout << "Server running on " << portStr << std::endl;
    serverThread = new std::thread([this](){ this->serverWork(); });
    std::string input;
    while(isRunning)
    {
        std::cin >> input;
        if(input == "exit")
        {
            isRunning = false;
        }
        else
        {
            std::cout << "Undefined command" << std::endl;
        }
    }

    serverThread->join();
    for(std::thread* t : clientThreads)
        t->join();
    close(sd);
    sd = -1;
}

void ShuraServer::clientWork(int fd)
{
    std::string playerName;
    while(isRunning)
    {
        nlohmann::json msg;
        int msgSize;
        if(recv(fd, &msgSize, sizeof(int), MSG_WAITALL) == -1)
            break;
        std::vector<char> tmp(msgSize);
        if(recv(fd, tmp.data(), msgSize, MSG_WAITALL) == -1)
            break;
        msg = nlohmann::json::parse(tmp.data());
        nlohmann::json response = game;
        
        if(msg.contains("register") && !game["players"].contains(msg["register"]))
        {
            playerName = msg["register"];
            game["players"].emplace_back(playerName);
            response["priv"]["register"]=true;
            std::cout << "Player " << playerName << " joined the game.";
        }
        else
            response["priv"]["register"]=false;

        std::string data = response.dump();
        std::size_t len = data.length();
        write(fd, &len, sizeof(std::size_t));
        write(fd, data.c_str(), len);
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);
}
void ShuraServer::serverWork()
{
    while(isRunning)
    {
        sockaddr_in connectionInf = {};
        socklen_t infLen=sizeof(connectionInf);
        int fd = accept4(sd, (sockaddr*)&connectionInf, &infLen, SOCK_NONBLOCK);
        if(fd != -1)
        {
            std::cout << "New connection from " << inet_ntoa(connectionInf.sin_addr) << std::endl;
            clientThreads.push_back(new std::thread( [this, fd] () { this->clientWork(fd); } ));
        }
        else
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2000ms);
        }
    }
}
