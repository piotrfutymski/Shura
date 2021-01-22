#pragma once
#include "Engine.h"
#include "Explosion.h"

class Player;

class Bullet
{
public:

    Bullet(const sf::Vector2f & pos, const sf::Vector2f & sp)
    {
        position = pos;
        speed = sp;
    }

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onKill(Didax::Engine * eng);
    void addObstacles(std::vector<Didax::Entity_t*> obs);
    void addPlayers(std::vector<Didax::Animable<Player>*> pl);


private:

    bool isCollision(Didax::Engine * eng);

    sf::Vector2f speed;
    sf::Vector2f position;
    Didax::Sprite<Bullet> * me = nullptr;
    std::vector<Didax::Entity_t*> obstacles;
    std::vector<Didax::Animable<Player>*> players;

};