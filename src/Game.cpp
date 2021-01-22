#include "Game.h"

void Game::onUpdate(Didax::Engine * eng)
{
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

            if(x*x+y*y <200*200 && _playerArtifact->getGameObject()->getArtifactSafe() < 0)
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
       _playerMain->getGameObject()->setBulletAngle(_playerMain->getGameObject()->getBulletAngle() - dT*30);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       _playerMain->getGameObject()->setBulletAngle(_playerMain->getGameObject()->getBulletAngle() + dT*30);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)&&sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        _players[1]->getGameObject()->setMoveState(true, 45);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)&&sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        _players[1]->getGameObject()->setMoveState(true, 315);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)&&sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        _players[1]->getGameObject()->setMoveState(true, 135);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)&&sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        _players[1]->getGameObject()->setMoveState(true, 225);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        _players[1]->getGameObject()->setMoveState(true, 0);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        _players[1]->getGameObject()->setMoveState(true, 270);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        _players[1]->getGameObject()->setMoveState(true, 180);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        _players[1]->getGameObject()->setMoveState(true, 90);
    else
        _players[1]->getGameObject()->setMoveState(false, _players[1]->getGameObject()->getDirection());

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        _players[1]->getGameObject()->setBulletAngle(_players[1]->getGameObject()->getBulletAngle() - dT*80);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        _players[1]->getGameObject()->setBulletAngle(_players[1]->getGameObject()->getBulletAngle() + dT*80);
    

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
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(0), "Player1"));
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(1), "Player2"));
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(2), "Player3"));
    _players.push_back(eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(3), "Player4"));
    _playerMain = _players[0];

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
            auto e = eng->addEntity<Didax::Sprite<Artifact>>(std::make_shared<Artifact>(_floor[n]->getPosition()), "Artifact");
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
    nlohmann::json data;
    data[name]["W"] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    data[name]["A"] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    data[name]["S"] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    data[name]["D"] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    data[name]["left"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    data[name]["right"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    return data;
}
void Game::pull_Keys(const nlohmann::json& gameInfo)
{
    //p->getGameObject()->setMoveState(d)
}

void Game::setName(const std::string & n)
{
    name = n;
}

std::string Game::getName()
{
    return name;
}