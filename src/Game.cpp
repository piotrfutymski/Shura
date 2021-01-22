#include "Game.h"

void Game::onUpdate(Didax::Engine * eng)
{
    eng->setCameraPosition(_playerMain->getPosition() + sf::Vector2f{50,70});
    timer-=eng->getDeltaT();
    if(timer < 0.0f)
    {
        timer += 0.2f;
        spawn_bullets(eng);
    }

    for(auto p: _players)
    {
        if(p->getGameObject()->isHaveingArtifact())
            _playerArtifact = p;

        if(p != _playerArtifact && _playerArtifact != nullptr)
        {
            auto x = _playerArtifact->getPosition().x - p->getPosition().x;
            auto y = _playerArtifact->getPosition().y - p->getPosition().y;

            if(x*x+y*y <200*200)
            {
                p->getGameObject()->giveArtifact(eng);
                _playerArtifact->getGameObject()->stealArtifact();
                _playerArtifact->getGameObject()->reSpawn();
                _playerArtifact = p;
            }
        }
    }

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

void Game::spawn_bullets(Didax::Engine * eng)
{
    if(_playerArtifact == nullptr)
        return;

    std::vector<Didax::Entity_t *> obst;
    for(auto & e: _wall)
        obst.push_back(e);
    for(auto &p: _players)
        obst.push_back(p);

    std::shared_ptr<Bullet> t1 = std::make_shared<Bullet>(_playerArtifact->getPosition(),sf::Vector2f{-450.0f,-450.0f});
    std::shared_ptr<Bullet> t2 = std::make_shared<Bullet>(_playerArtifact->getPosition(),sf::Vector2f{450.0f,-450.0f});
    std::shared_ptr<Bullet> t3 = std::make_shared<Bullet>(_playerArtifact->getPosition(),sf::Vector2f{-450.0f,450.0f});
    std::shared_ptr<Bullet> t4 = std::make_shared<Bullet>(_playerArtifact->getPosition(),sf::Vector2f{450.0f,450.0f});

    auto e = eng->addEntity<Didax::Sprite<Bullet>>(t1,"bullet");
    e->getGameObject()->addObstacles(obst);
    e->getGameObject()->addPlayers(_players);
    e = eng->addEntity<Didax::Sprite<Bullet>>(t2,"bullet");
    e->getGameObject()->addObstacles(obst);
    e->getGameObject()->addPlayers(_players);
    e = eng->addEntity<Didax::Sprite<Bullet>>(t3,"bullet");
    e->getGameObject()->addObstacles(obst);
    e->getGameObject()->addPlayers(_players);
    e = eng->addEntity<Didax::Sprite<Bullet>>(t4,"bullet");
    e->getGameObject()->addObstacles(obst);
    e->getGameObject()->addPlayers(_players);
}