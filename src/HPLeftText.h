#pragma once

#include "Engine.h"

class HPLeftText
{
public:
    HPLeftText(int hp):_hp{hp}{}

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntityText();
        me->setFont("data/fonts/DKNorthumbria.otf");
        me->setPriority(100);
        me->setFontSize(16);
        me->setColor(sf::Color::Black);
        setHP(_hp);
    }

    void setHP(int hp)
    {
        _hp = hp;
        me->setText(sf::String(name)+ L":  "+sf::String(std::to_string(_hp)));
    }

    void setName(const std::string & n)
    {
        name = n;
        me->setText(sf::String(name)+ L":  "+sf::String(std::to_string(_hp)));
    }


private:

    Didax::Text<HPLeftText> * me;
    std::string name;
    int _hp;

};