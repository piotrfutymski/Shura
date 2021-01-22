#include "Engine.h"

namespace Didax
{

Engine::Engine(){}

int Engine::init(const std::string settingFilePath)
{
    srand(time(NULL));
    std::ifstream stream(settingFilePath);
    if(!stream.is_open())
    {
        std::cerr<<"Unable to open setting file\n";
        return -1;
    }
    try
    {
        stream >> _settings;
    }
    catch(const std::exception& e)
    {
        std::cerr<<"Unable to load json\n";
        return -1;
    }
    WindowConfig wconfig;
    if(wconfig.init(_settings) == -1)
    {
        std::cerr<<"Unable to read window config\n";
        return -1;
    }      
    _window.init(wconfig);
    _clock.restart();
    Input::setWindow(&_window.getWindow());
    _window.getWindow().setActive(false);
    AssetManager::loadAllAssets(_settings);
    return 0;
}

void Engine::run()
{
    _window.getWindow().setActive(true);
    while (_window.getWindow().isOpen())
	{
		update();
		render();
		input();
	}
}

void Engine::update()
{
	auto time = _clock.getElapsedTime();
	_clock.restart();
	_deltaT = time.asSeconds();
	// updating
    for(size_t i = 0; i < _entities.size(); i++)
    {
        _entities[i]->update(_deltaT);        
    }
    if(_entityAdded != 0)
    {
        sortEntities();
        _entityAdded = 0;
    }

    bool ereased = false; 
    for (auto it = _entities.begin(); it != _entities.end(); )
    {
        if ((*it)->getToKill())
        {
            (*it)->kill();
            it = _entities.erase(it);
        }           
        else
            ++it;
    } 
    if(ereased)
        sortEntities();
	
}
	
void Engine::render()
{
	_window.getWindow().clear(sf::Color{ 0,0,0,255 });

	//rendering
    
    for(auto & en: _entities)
	    _window.getWindow().draw(*en.get());

	//

	_window.render();
}
	
void Engine::input()
{
	sf::Event event;

	while (_window.getWindow().pollEvent(event))
	{
		_window.processEvent(event);

		if (event.type == sf::Event::EventType::KeyPressed && event.key.code == sf::Keyboard::Escape)
			_window.getWindow().close();			

		for(auto & en: _entities)
            en->input(event);
	}
}

float Engine::getDeltaT()
{
    return _deltaT;
}

void Engine::sortEntities()
{
    std::sort(_entities.begin(), _entities.end(), [](auto & left, auto & right){
        return left->getPriority() < right->getPriority();
    });
}

}