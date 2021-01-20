#pragma once
#include "SFML/Graphics.hpp"

namespace Didax
{

class Input
{
public:

	static void setWindow(sf::Window * w);

	static sf::Vector2i getMousePosition();

	static sf::Vector2i getRezolution();
private:

	static sf::Window * _window;
};

}