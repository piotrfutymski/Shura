#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "Entity.h"


namespace Didax
{



class Engine
{
    using EntityHolder_t = std::vector<std::unique_ptr<Entity>>;

    Engine();

    int init(const std::string settingFilePath);

    int run();



private:

    EntityHolder_t _entities;

    nlohmann::json _settings;

private:

    int input();
    int update();
    int render();

};

}