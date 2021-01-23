#pragma once
#include "Engine.h"
#include "Explosion.h"
#include "Player.h"

class Artifact
{
public:

    Artifact(const sf::Vector2f & pos, bool * ex)
    {
        position = pos;
        existsFlag = ex;
        *existsFlag = true;
    }

    void onUpdate(Didax::Engine * eng)
    {
        if(isCollision())
        {
            me->setToKill(); 
            *existsFlag = false;
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

    void setPosition(const sf::Vector2f & p)
    {
        position = p;
        me->setPosition(position);
    }

    sf::Vector2f getPosition()
    {
        return position;
    }
    

private:

    sf::Vector2f position;

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

    Didax::Sprite<Artifact> * me = nullptr;
    std::vector<Didax::Animable<Player>*> players;
    bool * existsFlag;

};