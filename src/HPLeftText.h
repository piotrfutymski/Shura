#pragma once

#include "Engine.h"

class HPLeftText
{
public:
    HPLeftText(int id, int hp):_id{id}, _hp{hp}{}

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntityText();
        me->setFont("data/fonts/DKNorthumbria.otf");
        me->setPriority(100);
        me->setFontSize(16);
        setHP(_hp);
    }

    void setHP(int hp)
    {
        _hp = hp;
        me->setText(L"Player " +  sf::String(std::to_string(_id))+ L":  "+sf::String(std::to_string(_hp)));
    }


private:

    Didax::Text<HPLeftText> * me;
    int _id;
    int _hp;

};