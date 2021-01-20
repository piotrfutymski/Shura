#include "Tile.h"

Tile::Tile(const std::string & tname, const sf::Vector2f & pos)
{
    _textureName = tname;
    _position = pos;
}

void Tile::onStart(Didax::Engine * eng)
{
    auto me = eng->getMyEntitySprite();
    me->setTexture(_textureName);
    me->setPosition(_position);
}

