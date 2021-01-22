#pragma once

#include "Entity.h"
#include "AssetManager.h"

namespace Didax
{

class Engine;

template <typename T>
class Text : public Entity<T>
{
public:

    Text(Engine * parent, const std::string & name, std::shared_ptr<T> gameObj) :Entity<T>(parent, name, gameObj)
    {}

    void setFont(const std::string & name)
    {
        _text.setFont(AssetManager::getAsset<FontAsset>(name)->_font);
    }

    void setText(const sf::String & text)
    {
        _text.setString(text);
        this->setSize(sf::Vector2f{_text.getGlobalBounds().width, _text.getGlobalBounds().height});
    }

    void setColor(const sf::Color & c)
    {
        _text.setFillColor(c);
    }

    void setFontSize(int s)
    {
        _text.setCharacterSize(s);
    }

    sf::Color getColor()const
    {
        return _text.getColor();
    }

protected:

    virtual void _update(double dt)
    {}
    virtual void _input(const sf::Event & evt)
    {}
    virtual void _draw(sf::RenderTarget & target, sf::RenderStates states)const
    {
        target.draw(_text, states);
    }
    virtual void updatePosition()
    {
        _text.setPosition(this->_position);
    }
	virtual void updateSize()
    {
    }

    sf::Text _text;

};

}