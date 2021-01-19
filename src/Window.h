#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "WindowConfig.h"

namespace Didax
{

class Window
{
public:

	Window();
	~Window();

	void init(const WindowConfig & config);

	sf::RenderWindow & getWindow();
	const sf::RenderWindow & getWindow()const;

	sf::VideoMode getVideoMode();
	void setVideoMode(const sf::VideoMode & mode);

	int getFramerateLimit()const;
	void setFramerateLimit(int limit);

	const std::string& getTitle()const;
	void setTitle(const std::string & title);

	bool getFoolscreen() const;
	void setFoolscreen(bool state);

	sf::Vector2i getRezolution();

	void recreate();

	void render();

	void processEvent(sf::Event event);

	void draw(const sf::Drawable & drawable, const sf::RenderStates & states = sf::RenderStates::Default);



private:

	sf::VideoMode _videoMode;

	int _framerateLimit;

	std::string _title;

	sf::Uint32 _windowStyle;

	sf::RenderWindow _window;

	sf::Vector2i _rezolution;

};

}