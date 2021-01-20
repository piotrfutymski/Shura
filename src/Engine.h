#pragma once

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <type_traits>

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include "Animable.h"
#include "Sprite.h"
#include "Scriptable.h"
#include "Window.h"
#include "Input.h"
#include "AssetManager.h"


#define getMyEntitySprite() getEntity<Didax::Sprite<std::remove_reference<decltype(*this)>::type>>(this);
#define getMyEntityAnimable() getEntity<Didax::Animable<std::remove_reference<decltype(*this)>::type>>(this);
#define getMyEntityScriptable() getEntity<Didax::Scriptable<std::remove_reference<decltype(*this)>::type>>(this);

namespace Didax
{

class Engine
{
public:

    using EntityHolder_t = std::vector<std::unique_ptr<Entity_t>>;

    Engine();

    int init(const std::string settingFilePath);

    void run();

    //

    float getDeltaT();

    template<typename T, typename GameObject>
    typename std::enable_if<std::is_base_of<Entity_t, T>::value, T>::type * addEntity(std::shared_ptr<GameObject> gameObj, const std::string & name)
    {
        auto entity = std::make_unique<T>(this, name, gameObj);
        auto res = entity.get();
        _entities.push_back(std::move(entity));
        sortEntities();
        return res;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<Entity_t, T>::value, T>::type * getEntityWithName(const std::string & name)
    {
        if (auto it = _find(name); it != _entities.end())
			return static_cast<typename std::enable_if<std::is_base_of<Entity_t, T>::value, T>::type *>(it->get());
		return nullptr;
    }

    template<typename T, typename GameObject>
    typename std::enable_if<std::is_base_of<Entity_t, T>::value, T>::type * getEntity(GameObject * obj)
    {
        if (auto it = _find<GameObject>(obj); it != _entities.end())
			return static_cast<typename std::enable_if<std::is_base_of<Entity_t, T>::value, T>::type *>(it->get());
		return nullptr;
    }

    void setCameraPosition(const sf::Vector2f & pos)
    {
        auto vi = _window.getWindow().getDefaultView();
        vi.setCenter(pos);
        _window.getWindow().setView(vi);
    }
    
    void moveCamera(const sf::Vector2f & delta)
    {
        auto vi = _window.getWindow().getView();
        vi.setCenter(vi.getCenter() + delta);
        _window.getWindow().setView(vi);
    }

    void sortEntities();

private:

    EntityHolder_t _entities;

    float _deltaT{0.0};

    nlohmann::json _settings;
    sf::Clock _clock;
    Window _window;


private:

    void input();
    void update();
    void render();

    
    

    template<typename GameObject>
	std::vector<std::unique_ptr<Entity_t>>::iterator _find(GameObject * obj)
	{
		return std::find_if(_entities.begin(), _entities.end(), [=](const auto & arg) {
            if(auto d_arg = dynamic_cast<Entity<GameObject>*>(arg.get()))
                return d_arg->getGameObject() == obj;
			return false;
		});
	}

    std::vector<std::unique_ptr<Entity_t>>::iterator _find(const std::string & name)
	{
		return std::find_if(_entities.begin(), _entities.end(), [=](const auto & arg) {
			return arg->getName() == name;
		});
	}

    

};

}