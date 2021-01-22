#pragma once

#include "Entity.h"
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

    void setAnimationAsset(const std::string & name)
    {
        _animation = AssetManager::getAsset<AnimationAsset>(name);
        _sprite.setTexture(AssetManager::getAsset<TextureAsset>(_animation->_texture)->_texture);
        this->setSize(_animation->_rectangleSize);
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

    bool playing()const
    {
        return _playing;
    }

    void resetAnimation()
    {
        _frameNum = 0;
        _elapsed = 0.0f;
        _sprite.setTextureRect(getActualRect());
    }

    void setColor(const sf::Color & c)
    {
        _sprite.setColor(c);
    }

    sf::Color getColor()const
    {
        return _sprite.getColor();
    }


protected:

    void nextFrame()
    {
        _elapsed = _elapsed - 1.0/_animation->_animationSpeeds[_animNum];
        if(_frameNum == _animation->_animationFrames[_animNum] -1)
        {
            _frameNum = 0;
            if(_playOnce)
                _playing = false;
        }          
        else
            _frameNum++;
        _sprite.setTextureRect(getActualRect());
    }

    sf::Vector2f rectSize();

    virtual void _update(double dT)
    {
        if(_playing)
            _elapsed += dT;
        if(_playing && _elapsed > 1.0/_animation->_animationSpeeds[_animNum])
            nextFrame();           
    }
    virtual void _input(const sf::Event & evt)
    {}
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        target.draw(_sprite, states);
    }

    virtual void updatePosition()
    {
        _sprite.setPosition(this->_position);
    }
	virtual void updateSize()
    {
        if (_sprite.getTexture() == nullptr)
		    return;
	    auto textureSize = _animation->_rectangleSize;
	    _sprite.setScale({ this->_size.x / textureSize.x, this->_size.y / textureSize.y });
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

