#pragma once
#include "Engine.h"

class Tile
{
public:

    Tile(const std::string & tname, const sf::Vector2f & pos);

    void onStart(Didax::Engine * eng);

private:

    std::string _textureName;
    sf::Vector2f _position;

};