#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <netdb.h>

#include "ShuraServer.h"

class ShuraClient
{
public:
    ShuraClient();
    void run(const char * ipStr, const char * portStr);
private:
    int sd;
    nlohmann::json game;
};