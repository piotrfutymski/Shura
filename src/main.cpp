#include <iostream>
#include "ShuraClient.h"
int main(int argc, char ** argv)
{
   if(argc == 3)
   {
      ShuraClient client;
      client.run(argv[1], argv[2]);
   }
   else if (argc == 2)
   {
      ShuraServer server;
      server.run(argv[1]);
   }
   else
      std::cout << "Bad args" << std::endl;
   return 0;
}

