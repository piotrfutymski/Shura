#pragma once

#include "Sprite.h"
#include "AssetManager.h"
#include "Engine.h"



namespace Didax
{

template <typename T>
class Animable : public Entity<T>
{
public:

    Animable(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj) :Entity<T>(parent, name, gameObj)
    {}


    void setTexture(const std::string & name)
    {
        _sprite.setTexture(AssetManager::getAsset<TextureAsset>(name)->_texture);
    }

    void setAnimationAsset(const std::string & name)
    {
        _animation = AssetManager::getAsset<AnimationAsset>(name);
        _sprite.setTexture(AssetManager::getAsset<TextureAsset>(_animation->_texture)->_texture);
        setAnimation(0);
        resetAnimation();
    }

    void setAnimation(int num)
    {
        if(_animation == nullptr || _animation->_animationFrames.size() <=size_t(num))
            return;
        _animNum = num;
        resetAnimation();
    }
    int getAnimation()const
    {
        return _animNum;
    }
    void setPlayOnce(bool p)
    {
        _playOnce = true;
    }
    bool getPlayOnce()const
    {
        return _playOnce;
    }

    void play()
    {
        _playing = true;
    }
    void stop()
    {
        _playing = false;
    }

    void resetAnimation()
    {
        _frameNum = 0;
        _elapsed = 0.0f;
        _sprite.setTextureRect(getActualRect());
    }

    void nextFrame()
    {
        _elapsed = _elapsed - 1.0/_animation->_animationSpeeds[_animNum];
        if(_frameNum == _animation->_animationFrames[_animNum] -1)
            _frameNum = 0;
        else
            _frameNum++;
        _sprite.setTextureRect(getActualRect());
    }

    void setPosition(float x, float y)
    {
        _sprite.setPosition(x,y);
    }
    void setPosition(const sf::Vector2f & pos)
    {
        _sprite.setPosition(pos);
    }
    void move(float dx, float dy)
    {
        _sprite.move(dx,dy);
    }
    void move(const sf::Vector2f & pos)
    {
        _sprite.move(pos);
    }
     sf::Sprite * getSprite()
    {
        return &_sprite;
    }

protected:

    sf::Vector2f rectSize();

    virtual void _update(double dT)
    {
        if(_playing)
            _elapsed += dT;
        if(_playing && _elapsed > 1.0/_animation->_animationSpeeds[_animNum])
            nextFrame();
        if(_playOnce && _frameNum == 0)
            _playing = false;
    }
    virtual void _input(const sf::Event & evt)
    {}
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        target.draw(_sprite, states);
    }

    AnimationAsset * _animation;
    bool _playOnce{false};
    bool _playing{false};
    int _animNum{0};
    int _frameNum{0};

    sf::Sprite _sprite;

    float _elapsed{0.0f};

    sf::IntRect getActualRect()const
    {
        sf::IntRect res;
        res.left = _frameNum * _animation->_rectangleSize.x;
        res.top = _animNum  * _animation->_rectangleSize.y;
        res.width = _animation->_rectangleSize.x;
        res.height = _animation->_rectangleSize.y;
        return res;
    }


};

}

