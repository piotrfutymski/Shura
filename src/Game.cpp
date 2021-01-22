#include "Game.h"

void Game::onUpdate(Didax::Engine * eng)
{
    if(_playerMain != nullptr)
        eng->setCameraPosition(_playerMain->getPosition() + sf::Vector2f{50,70});
    auto dT = eng->getDeltaT();

    for(auto p: _players)
    {
        if(p->getGameObject()->isHaveingArtifact())
            _playerArtifact = p;

        if(p != _playerArtifact && _playerArtifact != nullptr)
        {
            auto x = _playerArtifact->getPosition().x - p->getPosition().x;
            auto y = _playerArtifact->getPosition().y - p->getPosition().y;

            if(x*x+y*y <200*200 && _playerArtifact->getGameObject()->getArtifactSafe() < 0 && !p->getGameObject()->immune())
            {
                p->getGameObject()->giveArtifact();
                _playerArtifact->getGameObject()->stealArtifact();
                _playerArtifact->getGameObject()->reSpawn();
                _playerArtifact = p;
            }
        }

        if(p == _playerArtifact && p->getGameObject()->getArtifactTimmer() < 0)
        {
            std::vector<Didax::Entity_t *> obst;
            for(auto & e: _wall)
                obst.push_back(e);
            for(auto &p: _players)
                obst.push_back(p);
            p->getGameObject()->spawnBullets(obst, _players, eng);
        }
    }

    if(_playerMain == nullptr)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)&&sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        _playerMain->getGameObject()->setMoveState(true, 45);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)&&sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        _playerMain->getGameObject()->setMoveState(true, 315);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)&&sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        _playerMain->getGameObject()->setMoveState(true, 135);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)&&sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        _playerMain->getGameObject()->setMoveState(true, 225);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        _playerMain->getGameObject()->setMoveState(true, 0);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        _playerMain->getGameObject()->setMoveState(true, 270);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        _playerMain->getGameObject()->setMoveState(true, 180);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        _playerMain->getGameObject()->setMoveState(true, 90);
    else
        _playerMain->getGameObject()->setMoveState(false, _playerMain->getGameObject()->getDirection());

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
       _playerMain->getGameObject()->setBulletAngle(_playerMain->getGameObject()->getBulletAngle() - dT*90);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       _playerMain->getGameObject()->setBulletAngle(_playerMain->getGameObject()->getBulletAngle() + dT*90);
    

}
void Game::onStart(Didax::Engine * eng)
{
    createTilesInRectangle(sf::IntRect{1,1,38,38},"floor", eng);
    createTilesInRectangle(sf::IntRect{0,0,1,40},"wall", eng);
    createTilesInRectangle(sf::IntRect{1,0,39,1},"wall", eng);
    createTilesInRectangle(sf::IntRect{1,39,39,1},"wall", eng);
    createTilesInRectangle(sf::IntRect{39,1,1,38},"wall", eng);
    createTilesInRectangle(sf::IntRect{19,1,2,10},"wall", eng);
    createTilesInRectangle(sf::IntRect{19,29,2,10},"wall", eng);
    createTilesInRectangle(sf::IntRect{1,19,10,2},"wall", eng);
    createTilesInRectangle(sf::IntRect{29,19,10,2},"wall", eng);
    createTilesInRectangle(sf::IntRect{19,19,2,2},"wall", eng);
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(0, &_bullets), "Player1"));
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(1, &_bullets), "Player2"));
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(2, &_bullets), "Player3"));
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(3, &_bullets), "Player4"));
    for(auto p: _players)
    {
        p->getGameObject()->setGhost();
    }

    bool art = false;
    while(!art)
    {
        int n = rand()%_floor.size();
        bool ud = true;
        for(auto w: _wall)
        {
            if(w->getPosition() == _floor[n]->getPosition())
            {
                ud = false;
                break;
            }
        }
        if(ud)
        {
            art = true;
            auto e = eng->addEntity<Didax::Sprite<Artifact>>(std::make_shared<Artifact>(_floor[n]->getPosition(), _artifact), "Artifact");
            e->getGameObject()->addPlayers(_players);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        std::vector<Didax::Entity_t *> obst;
        for(auto & e: _wall)
            obst.push_back(e);
        for (int j = 0; j < 4; j++)
        {
            if(i!=j)
                obst.push_back(_players[j]);
        }
        _players[i]->getGameObject()->addObstacles(obst);
    }

}
void Game::onInput(Didax::Engine * eng, const sf::Event & e)
{

}
void Game::onKill(Didax::Engine * eng)
{

}

void Game::createTilesInRectangle(const sf::IntRect & rec, const std::string & name, Didax::Engine * eng)
{
    std::string tex;
    if(name == "floor")
        tex = "data/graphix/floor.png";
    else
        tex = "data/graphix/wall.png";

    for(int i = 0; i < rec.width; i++)
    {
        for(int j = 0; j < rec.height; j++)
        {
            std::shared_ptr<Tile> t = std::make_shared<Tile>(tex, sf::Vector2f{rec.left*100.0f+i*100.0f, rec.top*100.0f+j*100.0f});
            if(name == "floor")
            {
                auto e = eng->addEntity<Didax::Sprite<Tile>>(t,name+std::to_string(_floor.size()));
                e->setPriority(9);
                _floor.push_back(e);
            }              
            else
            {
                auto e = eng->addEntity<Didax::Sprite<Tile>>(t,name+std::to_string(_wall.size()));
                e->setPriority(10);
                _wall.push_back(e);
            }                        
        }
    }   
}


void Game::push_Keys(nlohmann::json& gameInfo) const
{
    gameInfo[name]["W"] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    gameInfo[name]["A"] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    gameInfo[name]["S"] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    gameInfo[name]["D"] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    gameInfo[name]["angle"] = _playerMain->getGameObject()->getBulletAngle();
}
void Game::pull_Keys(const nlohmann::json& gameInfo)
{
    for(auto p: _players)
    {
        std::string pName = p->getName();
        if(!gameInfo.contains(pName))
            break;
        if(gameInfo[pName]["W"] && gameInfo[pName]["D"])
            p->getGameObject()->setMoveState(true, 45.f);
        else if(gameInfo[pName]["D"] && gameInfo[pName]["S"])
            p->getGameObject()->setMoveState(true, 135.f);
        else if(gameInfo[pName]["S"] && gameInfo[pName]["A"])
            p->getGameObject()->setMoveState(true, 225.f);
        else if(gameInfo[pName]["A"] && gameInfo[pName]["W"])
            p->getGameObject()->setMoveState(true, 315.f);
        else if(gameInfo[pName]["W"])
            p->getGameObject()->setMoveState(true, 0.f);
        else if(gameInfo[pName]["D"])
            p->getGameObject()->setMoveState(true, 90.f);
        else if(gameInfo[pName]["S"])
            p->getGameObject()->setMoveState(true, 180.f);
        else if(gameInfo[pName]["A"])
            p->getGameObject()->setMoveState(true, 270.f);
        else 
            p->getGameObject()->setMoveState(true, p->getGameObject()->getDirection());

        p->getGameObject()->setBulletAngle(gameInfo[pName]["angle"]);
    }
}

void Game::setName(const std::string & n)
{
    name = n;
}

std::string Game::getName()
{
    return name;
}

void Game::getGameState(nlohmann::json& gameInfo)const       //SERVER
{

}
void Game::actualizeState(const nlohmann::json & gameInfo)   //CLIENT
{

}


void Game::addPlayer(const std::string & name)
{
    _players[playerCount]->getGameObject()->setNormal();
    _players[playerCount]->getGameObject()->setName(name);
    if(this->name == name)
        _playerMain = _players[playerCount];
    playerCount++;
}
void Game::removePlayer(const std::string & name)
{
    for (int i = 0; i < 4; i++)
    {
        if(_players[i]->getGameObject()->getName() == name)
            _players[i]->getGameObject()->setGhost();
    }
    
}