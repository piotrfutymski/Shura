#include "Game.h"

nlohmann::json Game::getServerJson()
{
    return serverJson[currServerJson];
}
nlohmann::json Game::getClientJson()
{
    return clientJson[currClientJson];
}

void Game::setServerJson(nlohmann::json & gameInfo)
{
    int curr = (currServerJson+1)%jsonBufferSize;
    serverJson[curr] = gameInfo;
    currServerJson = curr;
    newJson = true;
}
void Game::setClientJson(nlohmann::json & gameInfo)
{
    int curr = (currClientJson+1)%jsonBufferSize;

    clientJson[curr] = clientJson[currClientJson];
    clientJson[curr].merge_patch(gameInfo);

    currClientJson = curr;
    newJson = true;
}

void Game::onUpdate(Didax::Engine * eng)
{
    if(_playerMain != nullptr)
        eng->setCameraPosition(_playerMain->getPosition() + sf::Vector2f{50,70});
    auto dT = eng->getDeltaT();

    if(!client)
    {
    alive.clear();
    for(auto p: _players)
    {
        if(p->getGameObject()->getHP() > 0 && !p->getGameObject()->ghost)
            alive.push_back(p->getGameObject()->getName());
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
    }
    if(newJson)
        updateGameFromJson();
    else
    {
        for(auto p :_players)
        {
            if(p != _playerMain)
            {
                p->getGameObject()->setMoveState(p->getGameObject()->getIfMooving(), p->getGameObject()->getDirection());
            }
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
        updateServerJson();
    else
        updateClientJson();

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


void Game::updateClientJson()
{
    nlohmann::json tmp;

    tmp[std::to_string(_id)]["HP"] = _playerMain->getGameObject()->getHP();
    tmp[std::to_string(_id)]["angle"] = (int)(_playerMain->getGameObject()->getBulletAngle());
    tmp[std::to_string(_id)]["X"] = (int)(_playerMain->getPosition().x);
    tmp[std::to_string(_id)]["Y"] = (int)(_playerMain->getPosition().y);
    tmp[std::to_string(_id)]["moving"] = _playerMain->getGameObject()->getIfMooving();
    tmp[std::to_string(_id)]["direction"] = (int)(_playerMain->getGameObject()->getDirection());

    int curr = (currClientJson+1)%jsonBufferSize;
    clientJson[curr] = tmp;
    currClientJson = curr;

}

void Game::updateServerJson()   //SERVER
{
    nlohmann::json tmp;
    for(int i = 0; i < 4; i++)
    {   
        auto p = _players[i];
        tmp[std::to_string(i)]["HP"] = p->getGameObject()->getHP();
        tmp[std::to_string(i)]["X"] = p->getPosition().x;
        tmp[std::to_string(i)]["Y"] = p->getPosition().y;
        tmp[std::to_string(i)]["moving"] = p->getGameObject()->getIfMooving();
        tmp[std::to_string(i)]["direction"] = p->getGameObject()->getDirection();
        tmp[std::to_string(i)]["flittering"] = p->getGameObject()->flittering;
        tmp[std::to_string(i)]["flitteringTimer"] = p->getGameObject()->flitteringTimer;
        tmp[std::to_string(i)]["flitterinLeft"] = p->getGameObject()->flitteringLeft;
        tmp[std::to_string(i)]["haveArtifact"] = p->getGameObject()->haveArtifact;
        tmp[std::to_string(i)]["artifactTimer"] = p->getGameObject()->artifactTimer;
        tmp[std::to_string(i)]["artifactSafe"] = p->getGameObject()->artifactSafe;
        tmp[std::to_string(i)]["ghost"] = p->getGameObject()->ghost;
        tmp[std::to_string(i)]["name"] = p->getGameObject()->getName();
        
    } 
    if(alive.size() == 1)
    {
        tmp["win"]=true;
        tmp["winner"]=alive[0];
    }
    else
    {
        tmp["win"]=false;
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
    int curr = (currServerJson+1)%jsonBufferSize;
    serverJson[curr] = tmp;
    currServerJson = curr;
}

void Game::updateGameFromJson()
{
    newJson = false;
    if(client)
    {//updating client game
        nlohmann::json gameInfo = serverJson[currServerJson];
        for(int i = 0; i < 4; i++)
        {    
            auto p = _players[i];
            if(!MyJsonUtil::containsName(gameInfo, std::to_string(i)))
                continue;

            if(_id !=i)
            {
                p->setPosition(sf::Vector2f{gameInfo[std::to_string(i)]["X"], gameInfo[std::to_string(i)]["Y"]});   
                p->getGameObject()->setMoveState(gameInfo[std::to_string(i)]["moving"], gameInfo[std::to_string(i)]["direction"]);
                p->getGameObject()->setHP(gameInfo[std::to_string(i)]["HP"]);
            }
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
        if(MyJsonUtil::containsName(gameInfo, "bulltes"))
        {
        for (int i = 0; i < 500; i++)
        {
            if(MyJsonUtil::containsName(gameInfo["bulltes"], std::to_string(i)))
            {
                try
                {
                    std::string tmp = gameInfo["bulltes"].dump();
                _bullets[i]->getGameObject()->setPosition(sf::Vector2f{gameInfo["bulltes"][std::to_string(i)]["px"], gameInfo["bulltes"][std::to_string(i)]["py"]});
                _bullets[i]->getGameObject()->speed.x =  gameInfo["bulltes"][std::to_string(i)]["vx"];
                _bullets[i]->getGameObject()->speed.y =  gameInfo["bulltes"][std::to_string(i)]["vy"];
                }
                catch(const std::exception& e)
                {
                }
            }
            else
            {
                _bullets[i]->getGameObject()->setPosition(sf::Vector2f{-2000,-2000});
                _bullets[i]->getGameObject()->speed.x =  0;
            }            
        }
        }
    }
    else
    {//updating server game
        nlohmann::json gameInfo = clientJson[currClientJson];
        for(int i = 0; i < 4; i++)
        {     
            auto p = _players[i];

            if(!MyJsonUtil::containsName(gameInfo, std::to_string(i)))
                continue;
            p->getGameObject()->setHP(gameInfo[std::to_string(i)]["HP"]) ;
            p->getGameObject()->setBulletAngle(gameInfo[std::to_string(i)]["angle"]);
            p->setPosition(sf::Vector2f{gameInfo[std::to_string(i)]["X"], gameInfo[std::to_string(i)]["Y"]});   
            p->getGameObject()->setMoveState(gameInfo[std::to_string(i)]["moving"], gameInfo[std::to_string(i)]["direction"]);      
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