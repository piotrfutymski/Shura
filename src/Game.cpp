#include "Game.h"

void Game::onUpdate(Didax::Engine * eng)
{
    eng->setCameraPosition(_player->getPosition() + sf::Vector2f{50,70});
    timer-=eng->getDeltaT();
    angle += eng->getDeltaT()*30;
    if(angle > 360)
        angle -= 360;
    if(timer < 0.0f)
    {
        timer += 0.2f;
        spawn_bullets(eng);
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

    eng->setCameraPosition({1000,1000});

    _player = eng->addEntity<Didax::Animable<Player>>(std::make_shared<Player>(), "Player");
    _player->setPosition(1000,1000);
    _player->setPriority(20);

    std::vector<Didax::Entity_t *> obst;
    for(auto & e: _wall)
        obst.push_back(e);

    _player->getGameObject()->addObstacles(obst);

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
    std::vector<Didax::Entity_t *> obst;
    for(auto & e: _wall)
        obst.push_back(e);
    
    obst.push_back(_player);

    std::shared_ptr<Bullet> t1 = std::make_shared<Bullet>(sf::Vector2f{1880,1880},sf::Vector2f{-450.0f,-450.0f});
    std::shared_ptr<Bullet> t2 = std::make_shared<Bullet>(sf::Vector2f{2120,1880},sf::Vector2f{450.0f,-450.0f});
    std::shared_ptr<Bullet> t3 = std::make_shared<Bullet>(sf::Vector2f{1880,2120},sf::Vector2f{-450.0f,450.0f});
    std::shared_ptr<Bullet> t4 = std::make_shared<Bullet>(sf::Vector2f{2120,2120},sf::Vector2f{450.0f,450.0f});

    auto e = eng->addEntity<Didax::Sprite<Bullet>>(t1,"bullet");
    e->getGameObject()->addObstacles(obst);
    e = eng->addEntity<Didax::Sprite<Bullet>>(t2,"bullet");
    e->getGameObject()->addObstacles(obst);
    e = eng->addEntity<Didax::Sprite<Bullet>>(t3,"bullet");
    e->getGameObject()->addObstacles(obst);
    e = eng->addEntity<Didax::Sprite<Bullet>>(t4,"bullet");
    e->getGameObject()->addObstacles(obst);
}