#include "Input.h"

namespace Didax
{
void Input::setWindow(sf::Window * w)
{
	_window = w;
}

sf::Vector2i Input::getMousePosition()
{
	return sf::Mouse::getPosition(*_window);
}

sf::Vector2i Input::getRezolution()
{
	return (sf::Vector2i) _window->getSize();
}

sf::Window * Input::_window = nullptr;

}