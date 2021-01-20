#pragma once
#include "Engine.h"

class Player
{
public:

    void onUpdate(Didax::Engine * eng)
    {
        auto dT = eng->getDeltaT();
        bool beg = isCollision(eng);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            me->move({0,-dT*280});
            if(isCollision(eng) && !beg)
                me->move({0,dT*280});
        }         
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            me->move({-dT*280,0});
                if(isCollision(eng) && !beg)
                    me->move({dT*280,0});
        }            
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            me->move({0,dT*280});
                if(isCollision(eng) && !beg)
                    me->move({0,-dT*280});
        }         
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            me->move({dT*280,0});
                if(isCollision(eng) && !beg)
                    me->move({-dT*280,0});
        }
            
    }

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntityAnimable();
        me->setAnimationAsset("mario");
        me->setSize({60,60});
        me->play();
        
    }

    void onInput(Didax::Engine * eng, const sf::Event & e)
    {
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::W && wA)
        {
            me->setAnimation(6);           
            wA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::W)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(2);   
            wA = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::A && aA)
        {
            me->setAnimation(5);
            aA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::A)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(1);
            aA = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::S && sA)
        {
            me->setAnimation(4);           
            sA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::S)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(0);   
            sA = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::D && dA)
        {
            me->setAnimation(7);
            dA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::D)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(3);
            dA = true;
        }
    }

    bool isMoveKeyPressed()
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            return true;
        return false;
    }

    void addObstacles(std::vector<Didax::Entity_t*> obs)
    {
        obstacles = obs;
    }

    bool isCollision(Didax::Engine * eng)
    {
        for(auto o : obstacles)
        {
            if(o->getPosition().x - me->getPosition().x > 300 || o->getPosition().x - me->getPosition().x < -300)
                continue;
            if(o->getPosition().y - me->getPosition().y > 300 || o->getPosition().y - me->getPosition().y < -300)
                continue;

            if(o->dectectColision(static_cast<Didax::Entity_t*>(me)))
                return true;
        }
        return false;
    }

    bool wA = true;
    bool aA = true;
    bool sA = true;
    bool dA = true;

private:
    std::vector<Didax::Entity_t*> obstacles;
    Didax::Animable<Player> * me = nullptr;
};