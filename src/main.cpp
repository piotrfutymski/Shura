#include <chrono>
#include <thread>
#include "Engine.h"

class Gracz
{
public:
    void onUpdate(Didax::Engine * eng)
    {
        auto me = eng->getMyEntitySprite()
        auto dT = eng->getDeltaT();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            me->move(0,-dT*50);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            me->move(-dT*50,0);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            me->move(0,dT*50);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            me->move(dT*50,0);
        
    }
    void onStart(Didax::Engine * eng)
    {
        auto me = eng->getMyEntitySprite();
        me->setTexture("data/graphix/test.bmp");
        me->setPosition(500,500);
    }
    void onKill(Didax::Engine * eng)
    {
        std::cout<<"LOOSE!\n";
    }
};


int main()
{

    Didax::Engine engine;
    engine.init("data/settings.json");
    std::shared_ptr<Gracz> gracz = std::make_shared<Gracz>();
    engine.addEntity<Didax::Sprite<Gracz>>(gracz, "gracz");

    std::thread([&](){
        std::getchar();
        engine.getEntityWithName<Didax::Sprite<Gracz>>("gracz")->setToKill();
    }).detach();

    engine.run();

}

