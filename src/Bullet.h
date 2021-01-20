#pragma once
#include "Engine.h"
#include "Explosion.h"

class Bullet
{
public:

    Bullet(const sf::Vector2f & pos, const sf::Vector2f & sp)
    {
        position = pos;
        speed = sp;
    }

    void onUpdate(Didax::Engine * eng)
    {
        auto dT = eng->getDeltaT();
        position = position + dT*speed;
        me->setPosition(position);

        if(isCollision(eng))
            me->setToKill();              
    }

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntitySprite();
        me->setTexture("data/graphix/bullet.png");
        me->setPosition(position);
        me->setPriority(15);
    }

    void onKill(Didax::Engine * eng)
    {
       auto e = eng->addEntity<Didax::Animable<Explosion>>(std::make_shared<Explosion>(), "expl");
       e->setPosition(position);
    }

    void addObstacles(std::vector<Didax::Entity_t*> obs)
    {
        obstacles = obs;
    }

private:

    bool isCollision(Didax::Engine * eng)
    {
        for(auto o : obstacles)
        {
            if(o->getPosition().x - me->getPosition().x > 400 || o->getPosition().x - me->getPosition().x < -400)
                continue;
            if(o->getPosition().y - me->getPosition().y > 400 || o->getPosition().y - me->getPosition().y < -400)
                continue;

            if(o->dectectColision(static_cast<Didax::Entity_t*>(me)))
                return true;
        }
        return false;
    }

    sf::Vector2f speed;
    sf::Vector2f position;
    Didax::Sprite<Bullet> * me = nullptr;
    std::vector<Didax::Entity_t*> obstacles;


};