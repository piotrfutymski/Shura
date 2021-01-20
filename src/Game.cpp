#include "Game.h"

void Game::onUpdate(Didax::Engine * eng)
{
    eng->setCameraPosition(eng->getEntity<Didax::Animable<Player>>(_player.get())->getPosition() + sf::Vector2f{50,70});
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
    _player = std::make_shared<Player>();

    auto e = eng->addEntity<Didax::Animable<Player>>(_player, "Player");
    e->setPosition(1000,1000);

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
                _floor.push_back(t);
                auto e = eng->addEntity<Didax::Sprite<Tile>>(t,name+std::to_string(_floor.size()));
                e->setPriority(9);
            }              
            else
            {
                _wall.push_back(t);  
                auto e = eng->addEntity<Didax::Sprite<Tile>>(t,name+std::to_string(_wall.size()));
                e->setPriority(10);
            }                        
        }
    }   
}