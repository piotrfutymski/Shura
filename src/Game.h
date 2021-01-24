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

    Game(bool cl, std::string n, int id):client{cl}, name{n}, _id{id}, currServerJson(0), currClientJson(0), newJson(false) { isArtifact = new bool; *isArtifact = true;}
    ~Game(){delete isArtifact;}

    void onUpdate(Didax::Engine * eng);
    void onStart(Didax::Engine * eng);
    void onInput(Didax::Engine * eng, const sf::Event & e);
    void onKill(Didax::Engine * eng);

    nlohmann::json getServerJson();
    nlohmann::json getClientJson();
    void setServerJson(nlohmann::json & gameInfo);
    void setClientJson(nlohmann::json & gameInfo);

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

    static const int jsonBufferSize = 100;
    bool updatedArtPos = false;
    nlohmann::json serverJson[jsonBufferSize];
    nlohmann::json clientJson[jsonBufferSize];
    volatile int currServerJson;
    volatile int currClientJson;
    void updateServerJson();
    void updateClientJson();
    volatile bool newJson;

    void updateGameFromJson();


    void createTilesInRectangle(const sf::IntRect & rec, const std::string & name, Didax::Engine * eng);

};

