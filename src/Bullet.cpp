#include "Bullet.h"
#include "Player.h"

    void Bullet::onUpdate(Didax::Engine * eng)
    {
        auto dT = eng->getDeltaT();
        position = position + dT*speed;
        me->setPosition(position);

        if(isCollision(eng))
            me->setToKill();              
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
       auto e = eng->addEntity<Didax::Animable<Explosion>>(std::make_shared<Explosion>(), "expl");
       e->setPosition(position);
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
                for(auto p: players)
                {
                    if(o == static_cast<Didax::Entity_t*>(p))
                    {
                        if(!p->getGameObject()->isFlittering() && !p->getGameObject()->isHaveingArtifact())
                            p->getGameObject()->minusHP();
                        if(p->getGameObject()->isHaveingArtifact())
                            return false;
                    }
                }
                return true;
            }
                
        }
        return false;
    }