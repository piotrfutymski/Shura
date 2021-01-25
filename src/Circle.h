#pragma once

#include "Engine.h"

class Circle
{
public:
    Circle(){}

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntitySprite();
        me->setTexture("data/graphix/circle.png");
        me->setPriority(18);
        me->setSize(sf::Vector2f{400,400});
        me->setColor(sf::Color(255,255,255,150));
    }

private:

    Didax::Sprite<Circle> * me;
};