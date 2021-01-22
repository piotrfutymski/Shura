#pragma once
#include "Engine.h"
#include "Bullet.h"
#include "HPLeftText.h"
#define PI 3.14159265

class Player
{
public:

    Player(int id, std::vector<Didax::Sprite<Bullet>*> * b)
    {
        _id = id;
        _bullets = b;
    };

    void addObstacles(std::vector<Didax::Entity_t*> obs);

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);

    void setMoveState(bool moving, float direction);
    bool getIfMooving()const;
    float getDirection()const;
    void spawnBullets(const std::vector<Didax::Entity_t *> & obst, const std::vector<Didax::Animable<Player> *> & players, Didax::Engine * eng);
    float getArtifactTimmer()const;

    void reSpawn();
    void startFilttering();
    bool isFlittering();
    void giveArtifact();
    void stealArtifact();
    bool isHaveingArtifact();
    void minusHP();
    int getHP();
    float getArtifactSafe();

    void setBulletAngle(float a);
    float getBulletAngle();

    void setName(const std::string & name);
    std::string getName();

    void setGhost();
    void setNormal();
    bool immune();

    void setPlayers(std::vector<Didax::Animable<Player>*> pl)
    {
        players = pl;
    }

    bool moving{false};          
    float direction{0};
    int HP{10};

    float bulletAngle{0};

    bool flittering{false};
    float flitteringTimer{0.5};
    const float flitteringTime{0.5};
    int flitteringLeft{6};
    bool haveArtifact{false};
    float artifactTimer{0.5};
    float artifactSafe{5};

    bool ghost{false};

private:
    std::vector<Didax::Entity_t*> obstacles;
    std::vector<Didax::Sprite<Bullet>*> * _bullets{nullptr};
    std::vector<Didax::Animable<Player>*> players;
    Didax::Animable<Player> * me = nullptr;
    Didax::Text<HPLeftText> * hpleft = nullptr;

    std::string name;

    sf::Color basicColor;
    int _id;               // used to choose start position itd.
   

private:

    bool isCollision()
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

};