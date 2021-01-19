#pragma once

#include <SFML/Window.hpp>
#include <nlohmann/json.hpp>

namespace Didax
{

class WindowConfig
{
public:
	WindowConfig();

	int init(const nlohmann::json & j);

	int loadFromJson(const nlohmann::json & j);

	sf::VideoMode _mode;
	sf::Uint32 _windowStyle;
	int _framerateLimit;

};

}