#pragma once

#include <memory>
#include <mutex>
#include <SFML/Graphics.hpp>

#include "HasMember.h"

namespace Didax
{

class Engine;

class Entity_t: public sf::Drawable
{
public:
    Entity_t(Engine * parent, const std::string & name)
    {
        _parent = parent;
        _name = name;
    }

    virtual void update(double dT) = 0;
    virtual void input(const sf::Event & evt) = 0;
    virtual void kill() = 0;

    bool getToKill()const
    {
        return _toKill;
    }

    void setToKill()
    {
        _toKill = true;
    }

    void setPriority(int p)
    {
        _priority = p;
    }

    int getPriority()const
    {
        return _priority;
    }

    std::string getName()const
    {
        return _name;
    }

protected:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const = 0;

    std::string _name{};
    Engine * _parent{nullptr};

    bool _toKill{false};
    bool _added{true};
    int _priority{0};
};


template <typename T>
class Entity : public Entity_t
{
public:

    Entity(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj)
        :Entity_t(parent, name)
    {
        _gameObj = gameObj;      
    }

    virtual void update(double dt)
    {
        if(auto ptr = _gameObj.lock())
        {
            if(_added)
            {
                _added = false;
                if constexpr (detail::has_onStart<T, void(Engine *)>::value)
                    if(auto ptr = _gameObj.lock())
                        ptr->onStart(_parent);
            }
            if constexpr (detail::has_onUpdate<T, void(Engine *)>::value)
            {
                ptr->onUpdate(_parent);
            }           
            _update(dt);
        }
        else
        {
            _toKill = true;
        }    
    }

    virtual void input(const sf::Event & evt)
    {
        if(auto ptr = _gameObj.lock())
        {
            if constexpr (detail::has_onInput<T, void(Engine *, const sf::Event &)>::value)
            {
                ptr->onInput(_parent, evt);
            }
            _input(evt);
        }
    }

    virtual void kill()
    {
        if constexpr (detail::has_onKill<T, void(Engine *)>::value)
                if(auto ptr = _gameObj.lock())
                    ptr->onKill(_parent);
    }

    T * getGameObject()
    {
        if(auto ptr = _gameObj.lock())
            return ptr.get();
        return nullptr;
    }

protected:

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        _draw(target, states);
    }

    virtual void _update(double dt) = 0;
    virtual void _input(const sf::Event & evt) = 0;
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const = 0;

protected:

    std::weak_ptr<T> _gameObj;
};

}

