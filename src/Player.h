#pragma once
#include "Engine.h"
#include "Bullet.h"
#include "HPLeftText.h"
#include "Circle.h"
#define PI 3.14159265

class Player
{
public:

    Player(int id)
    {
        _id = id;
    };

    void addObstacles(std::vector<Didax::Entity_t*> obs);

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);

    
    void spawnBullets(std::vector<Didax::Sprite<Bullet> *> & bullets);
    float getArtifactTimmer()const;

    void reSpawn();
    void startFilttering();
    bool isFlittering();
    void giveArtifact();
    void stealArtifact();
    bool isHaveingArtifact();
    void minusHP();
    float getArtifactSafe();
    bool immune();

    void setPlayers(std::vector<Didax::Animable<Player>*> pl)
    {
        players = pl;
    }

    void setName(const std::string & name);
    std::string getName();


    void setMoveState(bool moving, float direction);
    bool getIfMooving()const;
    float getDirection()const;

    void setHP(int HP);
    int getHP();

    void setBulletAngle(float a);
    float getBulletAngle();

    bool flittering{false};
    float flitteringTimer{0.5};
    int flitteringLeft{6};


    void setHavingArtfact();

    bool haveArtifact{false};
    float artifactTimer{0.5};
    float artifactSafe{5};

    void setGhost();
    void setNormal();

    bool ghost{false};
    

private:
    std::vector<Didax::Entity_t*> obstacles;
    //std::vector<Didax::Sprite<Bullet>*> * _bullets{nullptr};
    std::vector<Didax::Animable<Player>*> players;
    Didax::Animable<Player> * me = nullptr;
    Didax::Text<HPLeftText> * hpleft = nullptr;
    Didax::Sprite<Circle> * circ = nullptr;

    std::string name;
    int _id;               // used to choose start position itd.

    sf::Color basicColor;
    
    bool moving{false};          
    float direction{0};
    int HP{10};

    float bulletAngle{0};
    const float flitteringTime{0.5};
   

private:

    Didax::Entity_t *  isCollision()
    {
        for(auto o : obstacles)
        {
            if(o->getPosition().x - me->getPosition().x > 300 || o->getPosition().x - me->getPosition().x < -300)
                continue;
            if(o->getPosition().y - me->getPosition().y > 300 || o->getPosition().y - me->getPosition().y < -300)
                continue;

            if(o->dectectColision(static_cast<Didax::Entity_t*>(me)))
            {   
                bool f = true;
                for(auto p: players)
                {
                    if(o == static_cast<Didax::Entity_t*>(p))
                    {
                        if(!p->getGameObject()->immune() && !immune())
                        {
                            return o;
                        }
                        else
                            f = false; 
                    }
                }
                if(f)
                    return o;
            }
                
        }
        return nullptr;
    }

};