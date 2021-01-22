#pragma once

#include "Engine.h"
#include "Tile.h"
#include "Player.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Artifact.h"
#include "math.h"

class Game
{
public:

    Game(bool cl):client{cl}{}

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onInput(Didax::Engine * eng, const sf::Event & e);
    void onKill(Didax::Engine * eng);

    void push_Keys(nlohmann::json& gameInfo) const;         //CLIENT
    void pull_Keys(const nlohmann::json& gameInfo);         //SERVER

    void getGameState(nlohmann::json& gameInfo)const;       //SERVER
    void actualizeState(const nlohmann::json & gameInfo);   //CLIENT

    void setName(const std::string & name);
    std::string getName();

    void addPlayer(const std::string & name);
    void removePlayer(const std::string & name);

private:

    bool client{false}; 
    std::string name;


    std::vector<Didax::Sprite<Tile>*> _floor;
    std::vector<Didax::Sprite<Tile>*> _wall;
    Didax::Animable<Player>* _playerMain;
    Didax::Animable<Player>* _playerArtifact{nullptr};
    std::vector<Didax::Animable<Player>*> _players;

    void createTilesInRectangle(const sf::IntRect & rec, const std::string & name, Didax::Engine * eng);

};

