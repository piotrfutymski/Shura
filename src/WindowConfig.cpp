
#include "WindowConfig.h"

namespace Didax
{

WindowConfig::WindowConfig():_mode{640,480}, _windowStyle{sf::Style::Default}, _framerateLimit{60}
{
}

int WindowConfig::init(const nlohmann::json & j)
{
	return loadFromJson(j);
}

int WindowConfig::loadFromJson(const nlohmann::json & j)
{
	try 
	{
		auto windSet = j["windowSettings"];

		_mode.width = windSet["videoModeWidth"];
		_mode.height = windSet["videoModeHeight"];

		_windowStyle = windSet["windowStyle"];
		_framerateLimit = windSet["framerateLimit"];
			
	}
	catch(const std::exception & e)
	{
		return -1;
	}

    return 0;
}

}