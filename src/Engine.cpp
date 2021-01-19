#include "Engine.h"

namespace Didax
{

Engine::Engine(){}

int Engine::init(const std::string settingFilePath)
{
    srand(time(NULL));

    std::ifstream stream(settingFilePath);

    if(!stream.is_open())
    {
        // TO DO - Obsługa błędu
        return -1;
    }

    stream >> _settings;

    return 0;
}


}