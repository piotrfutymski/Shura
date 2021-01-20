#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <vector>

class ShuraServer
{
public:
    static int getDefaultPort() { return defaultPort; };

    ShuraServer();
    void run();

private:
    static const int defaultPort = 29999;

    sockaddr_in localAddress;
    int sd;

    void start();
};