#pragma once
#include "nlohmann/json.hpp"

class MyJsonUtil
{
public:
static bool containsName(const nlohmann::json & players, const std::string & name)
{
    for (auto& [key, val] : players.items())
    {
        if(key == name)
            return true;
        if(val.is_string() && val == name)
            return true;
    }
    return false;
}
static void deleteName(nlohmann::json & players, const std::string & name)
{
    for(nlohmann::json::iterator it = players.begin(); it != players.end(); )
    {
        if((*it) == name)
            players.erase(it);
        else
            it++;       
    }
}
};