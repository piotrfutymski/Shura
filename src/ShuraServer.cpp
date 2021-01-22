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
    fcntl(sd, F_SETFL, O_NONBLOCK, 1);
    isRunning = true;

    std::cout << "Server running on " << portStr << std::endl;
    serverThread = new std::thread([this](){ this->serverWork(); });
    std::string input;
    std::cout<< "Choose your hero name: ";
    std::string name;
    std::cin>>name;
    gameInfo["players"].emplace_back(name);
    runGame(name);
    isRunning = false;
    serverThread->join();
    delete serverThread;
    for(std::thread* t : clientThreads)
    {
        delete t;
        t->join();
    }
    clientThreads.clear();      
    close(sd);
    sd = -1;
}

void ShuraServer::clientWork(int fd)
{
    std::string playerName="";
    while(isRunning)
    {
        nlohmann::json msg;
        int msgSize;
        if(recv(fd, &msgSize, sizeof(int), MSG_WAITALL) == -1)
            break;
        std::vector<char> tmp(msgSize);
        if(recv(fd, tmp.data(), msgSize, MSG_WAITALL) == -1)
            break;
        try{
            msg = nlohmann::json::parse(tmp.data());
        }
        catch(const std::exception & e)
        {
            break;
        }
        
        nlohmann::json response = gameInfo;
        
        if(msg.contains("register") && !gameInfo["players"].contains(msg["register"]))
        {
            playerName = msg["register"];
            gameInfo["players"].emplace_back(playerName);
            response["priv"]["register"]=true;
            std::cout << "Player " << playerName << " joined the game.";
            game->addPlayer(playerName);
        }
        else
            response["priv"]["register"]=false;

        std::string data = response.dump();
        int len = data.length();
        write(fd, &len, sizeof(int));      
        write(fd, data.c_str(), len);             
    }

    game->removePlayer(playerName);
    shutdown(fd, SHUT_RDWR);
    close(fd);
}
void ShuraServer::serverWork()
{
    while(isRunning)
    {
        sockaddr_in connectionInf = {};
        socklen_t infLen=sizeof(connectionInf);
        int fd = accept(sd, (sockaddr*)&connectionInf, &infLen);
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

void ShuraServer::runGame(const std::string & name)
{
    Didax::Engine engine;
    engine.init("data/settings.json");
    game = std::make_shared<Game>(false);
    game->setName(name);
    engine.addEntity<Didax::Scriptable<Game>>(game, "Game");
    game->addPlayer(name);
    engine.run();
}