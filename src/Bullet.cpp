#include "Bullet.h"
#include "Player.h"

    void Bullet::onUpdate(Didax::Engine * eng)
    {
        auto dT = eng->getDeltaT();
        position = position + dT*speed;
        me->setPosition(position);

        if(isCollision(eng))
        {
            auto e = eng->addEntity<Didax::Animable<Explosion>>(std::make_shared<Explosion>(), "expl");
            e->setPosition(position);           
            setPosition(sf::Vector2f{-2000,-2000});
            speed.x = 0;

        }
                          
    }

    void Bullet::onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntitySprite();
        me->setTexture("data/graphix/bullet.png");
        me->setPosition(position);
        me->setPriority(15);
    }
    void Bullet::onKill(Didax::Engine * eng)
    {      
    }

    void Bullet::addObstacles(std::vector<Didax::Entity_t*> obs)
    {
        obstacles = obs;
    }

    void Bullet::addPlayers(std::vector<Didax::Animable<Player>*> pl)
    {
        players = pl;
    }

    bool Bullet::isCollision(Didax::Engine * eng)
    {
        for(auto o : obstacles)
        {
            if(o->getPosition().x - me->getPosition().x > 400 || o->getPosition().x - me->getPosition().x < -400)
                continue;
            if(o->getPosition().y - me->getPosition().y > 400 || o->getPosition().y - me->getPosition().y < -400)
                continue;

            if(o->dectectColision(static_cast<Didax::Entity_t*>(me)))
            {   
                bool f = true;
                for(auto p: players)
                {
                    if(o == static_cast<Didax::Entity_t*>(p))
                    {
                        if(!p->getGameObject()->immune())
                        {
                            p->getGameObject()->minusHP();
                            return true;
                        }
                        else
                            f = false; 
                    }
                }
                return f;
            }
                
        }
        return false;
    }

    void Bullet::setPosition(const sf::Vector2f & p)
    {
        position = p;
        me->setPosition(position);
    }