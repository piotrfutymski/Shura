#pragma once
#include "Engine.h"
#include "Explosion.h"

class Player;

class Bullet
{
public:

    Bullet(const sf::Vector2f & pos, const sf::Vector2f & sp, std::vector<Didax::Sprite<Bullet> *> * b)
    {
        bullets = b;
        position = pos;
        speed = sp;
    }

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onKill(Didax::Engine * eng);
    void addObstacles(std::vector<Didax::Entity_t*> obs);
    void addPlayers(std::vector<Didax::Animable<Player>*> pl);

    sf::Vector2f speed;
    sf::Vector2f position;

private:

    bool isCollision(Didax::Engine * eng);

    Didax::Sprite<Bullet> * me = nullptr;
    std::vector<Didax::Entity_t*> obstacles;
    std::vector<Didax::Animable<Player>*> players;
    std::vector<Didax::Sprite<Bullet> *> * bullets;

};