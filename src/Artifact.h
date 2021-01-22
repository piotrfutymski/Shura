#pragma once
#include "Engine.h"
#include "Explosion.h"
#include "Player.h"

class Artifact
{
public:

    Artifact(const sf::Vector2f & pos)
    {
        position = pos;
    }

    void onUpdate(Didax::Engine * eng)
    {
        if(isCollision(eng))
            me->setToKill();              
    }

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntitySprite();
        me->setTexture("data/graphix/artifact.png");
        me->setPosition(position);
        me->setPriority(15);
    }

    void addPlayers(std::vector<Didax::Animable<Player>*> pl)
    {
        players = pl;
    }

private:

    bool isCollision(Didax::Engine * eng)
    {
        for(auto o : players)
        {
            if(o->getPosition().x - me->getPosition().x > 400 || o->getPosition().x - me->getPosition().x < -400)
                continue;
            if(o->getPosition().y - me->getPosition().y > 400 || o->getPosition().y - me->getPosition().y < -400)
                continue;

            if(static_cast<Didax::Entity_t*>(o)->dectectColision(static_cast<Didax::Entity_t*>(me)))
            {   
                o->getGameObject()->giveArtifact(eng);
                return true;
            }              
        }
        return false;
    }

    sf::Vector2f position;
    Didax::Sprite<Artifact> * me = nullptr;
    std::vector<Didax::Animable<Player>*> players;

};