#pragma once
#include "Engine.h"

class Player
{
public:

    void onUpdate(Didax::Engine * eng)
    {
        auto me = eng->getMyEntityAnimable()
        auto dT = eng->getDeltaT();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            me->move({0,-dT*180});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            me->move({-dT*180,0});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            me->move({0,dT*180});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            me->move({dT*180,0});
        
    }

    void onStart(Didax::Engine * eng)
    {
        auto me = eng->getMyEntityAnimable();
        me->setAnimationAsset("mario");
        me->setPriority(20);
        eng->sortEntities();
    }

    void onInput(Didax::Engine * eng, const sf::Event & e)
    {
        auto me = eng->getMyEntityAnimable();
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::D && w)
        {
            me->setAnimation(0);
            me->play();
            w = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::D)
        {
            me->stop();
            w = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::A && w)
        {
            me->setAnimation(1);
            me->play();
            w = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::A)
        {

            me->stop();
            w = true;
        }
    }

    bool w = true;

private:

    
};