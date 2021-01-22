#pragma once
#include "Engine.h"

class Player
{
public:

    Player(int id): _id{id}{};

    void onUpdate(Didax::Engine * eng)
    {
        auto dT = eng->getDeltaT();
        bool beg = isCollision(eng);
        if(_id == 0){
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
        }}
        if(_id == 1){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            me->move({0,-dT*280});
            if(isCollision(eng) && !beg)
                me->move({0,dT*280});
        }         
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            me->move({-dT*280,0});
                if(isCollision(eng) && !beg)
                    me->move({dT*280,0});
        }            
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            me->move({0,dT*280});
                if(isCollision(eng) && !beg)
                    me->move({0,-dT*280});
        }         
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            me->move({dT*280,0});
                if(isCollision(eng) && !beg)
                    me->move({-dT*280,0});
        }}
        if(HP == 0)
        {
            HP = 2;
            reSpawn();
        }
        if(flittering)
        {
            flitteringTimer -= dT;
            if(flitteringTimer < 0)
            {
                flitteringTimer += 0.2;
                flitteringLeft--;
                if(flitteringLeft % 2)
                    me->setColor(sf::Color(255,255,255,125));
                else
                    me->setColor(sf::Color::White);
                if(flitteringLeft == 0)
                    flittering = false;
            }
        }
    }

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntityAnimable();
        me->setAnimationAsset("mario");
        me->setSize({60,60});
        me->setPriority(20);
        me->play();
        reSpawn();       
    }

    void onInput(Didax::Engine * eng, const sf::Event & e)
    {
        if(_id==0)
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
        if(_id==1)
        {
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::I && wA)
        {
            me->setAnimation(6);           
            wA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::I)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(2);   
            wA = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::J && aA)
        {
            me->setAnimation(5);
            aA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::J)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(1);
            aA = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::K && sA)
        {
            me->setAnimation(4);           
            sA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::K)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(0);   
            sA = true;
        }
        if(e.type == sf::Event::KeyPressed && e.key.code ==sf::Keyboard::L && dA)
        {
            me->setAnimation(7);
            dA = false;
        }
        if(e.type == sf::Event::KeyReleased && e.key.code ==sf::Keyboard::L)
        {
            if( ! isMoveKeyPressed())
                me->setAnimation(3);
            dA = true;
        }
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

    void reSpawn()
    {
        if(_id == 0)
            me->setPosition(2150,2000);
        else if(_id == 1)
            me->setPosition(1800,2000);
        else if(_id == 2)
            me->setPosition(2000,2100);
        else if(_id == 3)
            me->setPosition(2000,1800);
        else
            me->setPosition(-1000, -1000);
        
    }

    void startFilttering()
    {
        flittering = true;
        flitteringTimer = 0.2;
        flitteringLeft = 6;
    }

    bool isFlittering()
    {
        return flittering;
    }

    void giveArtifact(Didax::Engine * eng)
    {
        haveArtifact = true;
        if(me == nullptr)
            me = eng->getMyEntityAnimable();
        me->setColor(sf::Color(255,125,125));
    }

    void stealArtifact()
    {
        haveArtifact = false;
        me->setColor(sf::Color::White);
    }   

    bool isHaveingArtifact()
    {
        return haveArtifact;
    }

    void minusHP()
    {
        HP--;
        startFilttering();
    }

private:
    std::vector<Didax::Entity_t*> obstacles;
    Didax::Animable<Player> * me = nullptr;

    int _id;

    bool wA = true;
    bool aA = true;
    bool sA = true;
    bool dA = true;


    int HP{2};
    bool flittering{false};
    float flitteringTimer{0.2};
    int flitteringLeft{6};

    bool haveArtifact{false};

};