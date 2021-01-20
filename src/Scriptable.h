#pragma once

#include "Entity.h"
#include "AssetManager.h"

namespace Didax
{

class Engine;

template <typename T>
class Scriptable : public Entity<T>
{
public:

    Scriptable(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj) :Entity<T>(parent, name, gameObj)
    {}

protected:

    virtual void _update(double dt)
    {}
    virtual void _input(const sf::Event & evt)
    {}
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const
    {}

};

}