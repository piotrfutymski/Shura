#pragma once

#include "Engine.h"
#include "Tile.h"
#include "Player.h"

class Game
{
public:

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onInput(Didax::Engine * eng, const sf::Event & e);
    void onKill(Didax::Engine * eng);


private:

    std::vector<std::shared_ptr<Tile>> _floor;
    std::vector<std::shared_ptr<Tile>> _wall;
    std::shared_ptr<Player> _player;

    void createTilesInRectangle(const sf::IntRect & rec, const std::string & name, Didax::Engine * eng);

};

