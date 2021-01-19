#include "Window.h"

namespace Didax
{


Window::Window()
{
}

Window::~Window()
{
}

void Window::init(const WindowConfig & config)
{
	_videoMode = config._mode;
	_windowStyle = config._windowStyle;
	if (_windowStyle == sf::Style::Fullscreen)
		_videoMode = sf::VideoMode::getFullscreenModes()[0];
	_framerateLimit = config._framerateLimit;
	_rezolution = sf::Vector2i{ (int)_videoMode.width, (int)_videoMode.height };
	recreate();
}

sf::RenderWindow & Window::getWindow()
{
	return _window;
}

const sf::RenderWindow & Window::getWindow() const
{
	return _window;
}

sf::VideoMode Window::getVideoMode()
{
	return _videoMode;
}

void Window::setVideoMode(const sf::VideoMode & mode)
{
	_videoMode = mode;
	recreate();
}

int Window::getFramerateLimit() const
{
	return _framerateLimit;
}

void Window::setFramerateLimit(int limit)
{
	_framerateLimit = limit;
	_window.setFramerateLimit(limit);
}

const std::string & Window::getTitle() const
{
	return _title;
}

void Window::setTitle(const std::string & title)
{
	_title = title;
	_window.setTitle(title);
}

bool Window::getFoolscreen() const
{
	return (_windowStyle & sf::Style::Fullscreen);
}

void Window::setFoolscreen(bool state)
{
	if (state) 
		_windowStyle = sf::Style::Fullscreen;
	else 
		_windowStyle = sf::Style::Default;

	recreate();
}

sf::Vector2i Window::getRezolution()
{
	return _rezolution;
}

void Window::recreate()
{
	_window.create(_videoMode, _title, _windowStyle);
	//_window.setVerticalSyncEnabled(true);
	setFramerateLimit(_framerateLimit);
}

void Window::render()
{
	_window.display();
}

void Window::processEvent(sf::Event event)
{
	if (event.type == sf::Event::Closed)
	{
		_window.close();
	}
}

void Window::draw(const sf::Drawable & drawable, const sf::RenderStates & states)
{
	_window.draw(drawable, states);
}

}