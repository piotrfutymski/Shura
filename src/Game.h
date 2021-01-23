#pragma once

#include "Engine.h"
#include "Tile.h"
#include "Player.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Artifact.h"
#include "math.h"
#include <thread>

class Game
{
public:

    Game(bool cl, std::string n, int id):client{cl}, name{n}, _id{id}, currJson(0) { isArtifact = new bool; *isArtifact = true;}
    ~Game(){delete isArtifact;}

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onInput(Didax::Engine * eng, const sf::Event & e);
    void onKill(Didax::Engine * eng);
    nlohmann::json getGameJson();

    void push_Keys(nlohmann::json & gameInfo);         //CLIENT
    void pull_Keys(nlohmann::json & gameInfo);         //SERVER

    void actualizeState(nlohmann::json & gameInfo);   //CLIENT

    void addPlayer(const std::string & name, int id);
    int removePlayer(const std::string & name);


private:
    
    bool client{false}; 
    std::string name;
    int _id;

    std::vector<Didax::Sprite<Tile>*> _floor;
    std::vector<Didax::Sprite<Tile>*> _wall;
    Didax::Animable<Player>* _playerMain{nullptr};
    Didax::Animable<Player>* _playerArtifact{nullptr};
    std::vector<Didax::Animable<Player>*> _players;
    bool * isArtifact;
    Didax::Sprite<Artifact>* artifact;
    std::vector<Didax::Sprite<Bullet>*>_bullets;

    bool updatedArtPos = false;
    nlohmann::json gameJson[2];
    int currJson;
    void updateGameJson();
    void createTilesInRectangle(const sf::IntRect & rec, const std::string & name, Didax::Engine * eng);

};

