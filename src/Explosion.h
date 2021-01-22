#pragma once
#include "Engine.h"

class Explosion
{
public:

    void onUpdate(Didax::Engine * eng)
    {
        if(!me->playing())
        {
            auto a = me->getAnimation();
            me->setAnimation(a+1);
            if(a == me->getAnimation())
                me->setToKill();
            me->play();
        }        
    }

    void onStart(Didax::Engine * eng)
    {
        me = eng->getMyEntityAnimable();
        me->setAnimationAsset("explosion");
        me->setSize({50,50});
        me->setPriority(25);
        me->setPlayOnce(true);
        me->play();        
    }


private:
    Didax::Animable<Explosion> * me = nullptr;

};