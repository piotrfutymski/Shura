#pragma once
#include "Engine.h"
#include "Explosion.h"
#include "Player.h"

class Artifact
{
public:

    Artifact(const sf::Vector2f & pos, Didax::Sprite<Artifact> ** m)
    {
        m = &me;
        position = pos;
    }

    void onUpdate(Didax::Engine * eng)
    {
        if(isCollision())
        {
            me->setToKill(); 
            me = nullptr;
        }          
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

    bool isCollision()
    {
        for(auto o : players)
        {
            if(o->getPosition().x - me->getPosition().x > 400 || o->getPosition().x - me->getPosition().x < -400)
                continue;
            if(o->getPosition().y - me->getPosition().y > 400 || o->getPosition().y - me->getPosition().y < -400)
                continue;

            if(static_cast<Didax::Entity_t*>(o)->dectectColision(static_cast<Didax::Entity_t*>(me)))
            {
                if(!o->getGameObject()->immune())   
                    o->getGameObject()->giveArtifact();
                return true;
            }              
        }
        return false;
    }

    sf::Vector2f position;
    Didax::Sprite<Artifact> * me = nullptr;
    std::vector<Didax::Animable<Player>*> players;

};