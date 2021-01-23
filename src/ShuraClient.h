#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <netdb.h>
#include "Network.h"

#include "ShuraServer.h"
#include "Game.h"

class ShuraClient
{
public:
    ShuraClient();
    ShuraClient(ShuraClient&) = delete;
    ShuraClient(ShuraClient&&) = delete;
    ShuraClient& operator=(ShuraClient&) = delete;
    ShuraClient& operator=(ShuraClient&&) = delete;
    void run(const char * ipStr, const char * portStr);
private:
    bool isRunning;
    int sd;
    std::shared_ptr<Game> game;
    Didax::Engine engine;
    int _id;

    void prepareSocket(const char * ipStr, const char * portStr);
    std::string registerClient();
    void serverBinding();


};