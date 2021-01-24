#include "Game.h"

nlohmann::json Game::getGameJson()
{
    return gameJson[currJson];
}

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
            p->getGameObject()->spawnBullets(_bullets);
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
    
    if(!client)
        updateGameJson();

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
    for(auto p: _players)
    {
        p->getGameObject()->setGhost();
        p->getGameObject()->setPlayers(_players);
    }

    _players[_id]->getGameObject()->setName(name);
    _playerMain = _players[_id];
    _playerMain->getGameObject()->setNormal();

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
            auto e = eng->addEntity<Didax::Sprite<Artifact>>(std::make_shared<Artifact>(_floor[n]->getPosition(), isArtifact), "Artifact");
            e->getGameObject()->addPlayers(_players);
            artifact = e;
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

    std::vector<Didax::Entity_t *> obst;
            for(auto & e: _wall)
                obst.push_back(e);
            for(auto &p: _players)
                obst.push_back(p);

    for (int i = 0; i < 500; i++)
    {
        std::shared_ptr<Bullet> t1 = std::make_shared<Bullet>(sf::Vector2f{-2000,-2000},sf::Vector2f{0,0});
        auto e = eng->addEntity<Didax::Sprite<Bullet>>(t1,"bullet");
        e->getGameObject()->addObstacles(obst);
        e->getGameObject()->addPlayers(_players);
        _bullets.push_back(e);
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


void Game::push_Keys(nlohmann::json & gameInfo)
{
    //gameInfo[std::to_string(_id)]["W"] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    //gameInfo[std::to_string(_id)]["A"] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    //gameInfo[std::to_string(_id)]["S"] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    //gameInfo[std::to_string(_id)]["D"] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    gameInfo[std::to_string(_id)]["angle"] = (int)(_playerMain->getGameObject()->getBulletAngle());
    gameInfo[std::to_string(_id)]["X"] = (int)(_playerMain->getPosition().x);
    gameInfo[std::to_string(_id)]["Y"] = (int)(_playerMain->getPosition().y);
    gameInfo[std::to_string(_id)]["moving"] = _playerMain->getGameObject()->getIfMooving();
    gameInfo[std::to_string(_id)]["direction"] = (int)(_playerMain->getGameObject()->getDirection());
}
void Game::pull_Keys(nlohmann::json & gameInfo)
{
    for(int i = 0; i < 4; i++)
    {   
        auto p = _players[i];
        if(!gameInfo.contains(std::to_string(i)))
            continue;
        /*if(gameInfo[std::to_string(i)]["W"] && gameInfo[std::to_string(i)]["D"])
            p->getGameObject()->setMoveState(true, 45.f);
        else if(gameInfo[std::to_string(i)]["D"] && gameInfo[std::to_string(i)]["S"])
            p->getGameObject()->setMoveState(true, 135.f);
        else if(gameInfo[std::to_string(i)]["S"] && gameInfo[std::to_string(i)]["A"])
            p->getGameObject()->setMoveState(true, 225.f);
        else if(gameInfo[std::to_string(i)]["A"] && gameInfo[std::to_string(i)]["W"])
            p->getGameObject()->setMoveState(true, 315.f);
        else if(gameInfo[std::to_string(i)]["W"])
            p->getGameObject()->setMoveState(true, 0.f);
        else if(gameInfo[std::to_string(i)]["D"])
            p->getGameObject()->setMoveState(true, 90.f);
        else if(gameInfo[std::to_string(i)]["S"])
            p->getGameObject()->setMoveState(true, 180.f);
        else if(gameInfo[std::to_string(i)]["A"])
            p->getGameObject()->setMoveState(true, 270.f);
        else 
            p->getGameObject()->setMoveState(false, p->getGameObject()->getDirection());*/
        p->getGameObject()->setBulletAngle(gameInfo[std::to_string(i)]["angle"]);
        p->setPosition(sf::Vector2f{gameInfo[std::to_string(i)]["X"], gameInfo[std::to_string(i)]["Y"]});   
        p->getGameObject()->setMoveState(gameInfo[std::to_string(i)]["moving"], gameInfo[std::to_string(i)]["direction"]);      
    }
}

void Game::updateGameJson()     //SERVER
{
    nlohmann::json tmp;
    for(int i = 0; i < 4; i++)
    {   
        auto p = _players[i];

        tmp[std::to_string(i)]["X"] = p->getPosition().x;
        tmp[std::to_string(i)]["Y"] = p->getPosition().y;
        tmp[std::to_string(i)]["moving"] = p->getGameObject()->getIfMooving();
        tmp[std::to_string(i)]["direction"] = p->getGameObject()->getDirection();   
        tmp[std::to_string(i)]["HP"] = p->getGameObject()->getHP();
        tmp[std::to_string(i)]["flittering"] = p->getGameObject()->flittering;
        tmp[std::to_string(i)]["flitteringTimer"] = p->getGameObject()->flitteringTimer;
        tmp[std::to_string(i)]["flitterinLeft"] = p->getGameObject()->flitteringLeft;
        tmp[std::to_string(i)]["haveArtifact"] = p->getGameObject()->haveArtifact;
        tmp[std::to_string(i)]["artifactTimer"] = p->getGameObject()->artifactTimer;
        tmp[std::to_string(i)]["artifactSafe"] = p->getGameObject()->artifactSafe;
        tmp[std::to_string(i)]["ghost"] = p->getGameObject()->ghost;
        tmp[std::to_string(i)]["name"] = p->getGameObject()->getName();
        
    } 
    if(!(*isArtifact))
    {
        tmp["no_artifact"] = true;
        tmp["artifact_x"] = 0;
        tmp["artifact_y"] = 0;
    }
    else
    {
        tmp["no_artifact"] = false;
        tmp["artifact_x"] = (int)(artifact->getGameObject()->getPosition().x);
        tmp["artifact_y"] = (int)(artifact->getGameObject()->getPosition().y);
    }

    for (int i = 0; i < 500; i++)
    {
        if(_bullets[i]->getGameObject()->speed.x != 0)
        {
            tmp["bulltes"][std::to_string(i)]["px"] = (int)(_bullets[i]->getGameObject()->position.x);
            tmp["bulltes"][std::to_string(i)]["py"] = (int)(_bullets[i]->getGameObject()->position.y);
            tmp["bulltes"][std::to_string(i)]["vx"] = (int)(_bullets[i]->getGameObject()->speed.x);
            tmp["bulltes"][std::to_string(i)]["vy"] = (int)(_bullets[i]->getGameObject()->speed.y);
        }      
    }
    if(currJson == 0)
    {
        gameJson[1] = tmp;
        currJson = 1;
    }
    else
    {
        gameJson[0] = tmp;
        currJson = 0;
    }
}
void Game::actualizeState(nlohmann::json & gameInfo)   //CLIENT
{
    //std::cout<<gameInfo.dump()<<std::endl;

    for(int i = 0; i < 4; i++)
    {    
        auto p = _players[i];
        if(!gameInfo.contains(std::to_string(i)))
            continue;

        if(_id !=i)
        {
            p->setPosition(sf::Vector2f{gameInfo[std::to_string(i)]["X"], gameInfo[std::to_string(i)]["Y"]});   
            p->getGameObject()->setMoveState(gameInfo[std::to_string(i)]["moving"], gameInfo[std::to_string(i)]["direction"]);
        }
        p->getGameObject()->setHP(gameInfo[std::to_string(i)]["HP"]) ;
        p->getGameObject()->flittering = gameInfo[std::to_string(i)]["flittering"];
        p->getGameObject()->flitteringTimer = gameInfo[std::to_string(i)]["flitteringTimer"];
        p->getGameObject()->flitteringLeft = gameInfo[std::to_string(i)]["flitterinLeft"];
        p->getGameObject()->haveArtifact = gameInfo[std::to_string(i)]["haveArtifact"];
        p->getGameObject()->artifactTimer = gameInfo[std::to_string(i)]["artifactTimer"];
        p->getGameObject()->artifactSafe = gameInfo[std::to_string(i)]["artifactSafe"];
        if(gameInfo[std::to_string(i)]["ghost"])
            p->getGameObject()->setGhost();
        else       
            p->getGameObject()->setNormal();
        if(p->getGameObject()->getName() !=  gameInfo[std::to_string(i)]["name"])
            p->getGameObject()->setName(gameInfo[std::to_string(i)]["name"]);
    }

    if(!gameInfo["no_artifact"] && isArtifact && !updatedArtPos)
    {
        updatedArtPos = true;
        artifact->getGameObject()->setPosition({gameInfo["artifact_x"],gameInfo["artifact_y"]});
    }
    else if(gameInfo["no_artifact"] && isArtifact)
    {
        artifact->setToKill();
    }

    for (int i = 0; i < 500; i++)
    {
        if(gameInfo["bulltes"].contains(std::to_string(i)))
        {
            _bullets[i]->getGameObject()->setPosition(sf::Vector2f{gameInfo["bulltes"][std::to_string(i)]["px"], gameInfo["bulltes"][std::to_string(i)]["py"]});
            _bullets[i]->getGameObject()->speed.x =  gameInfo["bulltes"][std::to_string(i)]["vx"];
            _bullets[i]->getGameObject()->speed.y =  gameInfo["bulltes"][std::to_string(i)]["vy"];
        }
        else
        {
            _bullets[i]->getGameObject()->setPosition(sf::Vector2f{-2000,-2000});
            _bullets[i]->getGameObject()->speed.x =  0;
        }            
    }
}


void Game::addPlayer(const std::string & name, int id)
{
    _players[id]->getGameObject()->setNormal();
    _players[id]->getGameObject()->setName(name);
}

int Game::removePlayer(const std::string & name)
{
    for (int i = 0; i < 4; i++)
    {
        if(_players[i]->getGameObject()->getName() == name)
        {
            _players[i]->getGameObject()->setName("");
            _players[i]->getGameObject()->setGhost();
            return i;
        }
    }
    return -1;
}