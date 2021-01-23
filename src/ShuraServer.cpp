#include "ShuraServer.h"

ShuraServer::ShuraServer()
    :localAddress({}), sd(-1), game(), freeId({1,2,3}) {}

 void ShuraServer::prepareSocket(const char * portStr)
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
 }

void ShuraServer::run(const char * portStr)
{
    prepareSocket(portStr);

    std::cout<< "Choose your hero name: ";
    std::string name;
    std::cin>>name;
    gameInfo["players"].emplace_back(name);

    std::cout << "Server running on " << portStr << std::endl;
    joinWorkState = true;
    waitForInitState = true;
    clientWorkState = false;
    serverThread = new std::thread([this](){ this->joinWork(); });
    std::cout << "Click enter to start the game" <<std::endl;
    while(true)
    {
        std::string input;
        std::cin >> input;
        mut.lock();
        if(gameInfo["players"].size() < 2)
        {
            mut.unlock();
            std::cout << "Wait for other players to join" <<std::endl;
        }
        else
        {
            mut.unlock();
            break;
        }
    }
    mut.lock();
    joinWorkState = false;
    mut.unlock();
    serverThread->join();

    engine.init("data/settings.json");
    game = std::make_shared<Game>(false, name, 0);
    engine.addEntity<Didax::Scriptable<Game>>(game, "Game");
    game->addPlayer(name,0);

    clientWorkState = true;
    waitForInitState = false;

    engine.run();

    engine.lock();
    clientWorkState = false;
    engine.unlock();

    delete serverThread;
    for(std::thread* t : clientThreads)
    {
        t->join();
        delete t;
    }
    clientThreads.clear();      
    close(sd);
    sd = -1;
}

std::string ShuraServer::clientRegistration(int fd)
{
    std::string playerName="";
    while(playerName == "" && joinWorkState)
    {
        nlohmann::json msg = Network::receiveMsg(fd);
        nlohmann::json response;
        mut.lock();
        if(joinWorkState && msg.contains("register") && !gameInfo["players"].contains(msg["register"]))
        {
            playerName = msg["register"];
            gameInfo["players"].emplace_back(playerName);
            response["priv"]["register"]=true;
            response["priv"]["id"] = *(freeId.end()-1);
            freeId.pop_back();
            game->addPlayer(playerName,response["priv"]["id"]);
            std::cout << "Player " << playerName << " joined the game."<<std::endl;
            std::cout << "##########\n";
            std::cout << "Players:\n";
            for(nlohmann::json::iterator it = gameInfo["players"].begin(); it != gameInfo["players"].end(); it++)
                std::cout << (*it) << "\n";
            std::cout << "##########" << std::endl;
        }
        else
            response["priv"]["register"]=false;
        mut.unlock();
        Network::sendMsg(fd,response);
    }
    return playerName;
}

void ShuraServer::clientWork(int fd)
{
    nlohmann::json msg, response;
    while(waitForInitState)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
    }
    response["start"]=true;
    Network::sendMsg(fd,response);

    while(clientWorkState)
    {
        msg = Network::receiveMsg(fd);
        engine.lock();
        game->pull_Keys(msg);
        engine.lock();
        response = gameInfo;
        response.merge_patch(game->getGameJson());
        Network::sendMsg(fd, response);
    }

    response.clear();
    response["end"]=true;
    Network::sendMsg(fd,response);
}
void ShuraServer::joinWork()
{
    while(joinWorkState)
    {
        sockaddr_in connectionInf = {};
        socklen_t infLen=sizeof(connectionInf);
        int fd = accept(sd, (sockaddr*)&connectionInf, &infLen);
        if(fd != -1)
        {
            std::cout << "New connection from " << inet_ntoa(connectionInf.sin_addr) << std::endl;
            clientThreads.push_back(new std::thread( [this, fd] () 
            { 
                std::string playerName="";
                try
                {
                    playerName = this->clientRegistration(fd);
                    if(playerName != "")
                        this->clientWork(fd); 
                }
                catch(std::exception){;}
                if(playerName != "")
                {
                    mut.lock();
                    this->gameInfo["players"].erase(playerName);
                    freeId.push_back(game->removePlayer(playerName));
                    std::cout << "Player " << playerName << " left the game."<<std::endl;
                    std::cout << "##########\n";
                    std::cout << "Players:\n";
                    for(nlohmann::json::iterator it = gameInfo["players"].begin(); it != gameInfo["players"].end(); it++)
                        std::cout << (*it) << "\n";
                    std::cout << "##########" << std::endl;
                    mut.unlock();
                }

                shutdown(fd, SHUT_RDWR);
                close(fd);
            }));
        }
        else
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2000ms);
        }
    }
}