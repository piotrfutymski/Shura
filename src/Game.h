#pragma once

#include "Engine.h"
#include "Tile.h"
#include "Player.h"
#include "Bullet.h"
#include "Explosion.h"
#include "math.h"

class Game
{
public:

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onInput(Didax::Engine * eng, const sf::Event & e);
    void onKill(Didax::Engine * eng);


private:

    float timer = 0.5;
    float angle = 0;

    std::vector<Didax::Sprite<Tile>*> _floor;
    std::vector<Didax::Sprite<Tile>*> _wall;
    Didax::Animable<Player>* _player;

    void createTilesInRectangle(const sf::IntRect & rec, const std::string & name, Didax::Engine * eng);
    void spawn_bullets(Didax::Engine * eng);

};

