#pragma once

#include "Entity.h"
#include "AssetManager.h"

namespace Didax
{

class Engina;

template <typename T>
class Sprite : public Entity<T>
{
public:

    Sprite(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj) :Entity<T>(parent, name, gameObj)       
    {}

    sf::Sprite * getSprite()
    {
        return &_sprite;
    }

    void setTexture(const std::string & name)
    {
        auto t = AssetManager::getAsset<TextureAsset>(name);
        t->_texture.isRepeated();

        _sprite.setTexture(t->_texture);
    }
    void setPosition(float x, float y)
    {
        _sprite.setPosition(x,y);
    }
    void setPosition(const sf::Vector2f & pos)
    {
        _sprite.setPosition(pos);
    }
    void move(float dx, float dy)
    {
        _sprite.move(dx,dy);
    }
    void move(const sf::Vector2f & pos)
    {
        _sprite.move(pos);
    }

protected:

    virtual void _update()
    {}
    virtual void _input(const sf::Event & evt)
    {}
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        target.draw(_sprite, states);
    }

private:

    sf::Sprite _sprite;

};

}