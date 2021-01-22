#pragma once
#include "Engine.h"
#include "Bullet.h"
#include "HPLeftText.h"
#define PI 3.14159265

class Player
{
public:

    Player(int id): _id{id}{};

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

private:
    std::vector<Didax::Entity_t*> obstacles;
    Didax::Animable<Player> * me = nullptr;
    Didax::Text<HPLeftText> * hpleft = nullptr;

    sf::Color basicColor;
    int _id;               // used to choose start position itd.
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
                return true;
        }
        return false;
    }

};