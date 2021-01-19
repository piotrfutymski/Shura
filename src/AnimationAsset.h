#pragma once
#include "Asset.h"

#include <SFML/Graphics.hpp>

namespace Didax
{

class AnimationAsset :public Asset
{
public:
	AnimationAsset(const std::string & name) :Asset(name) {};
	~AnimationAsset() {};

	// Inherited via Asset
	virtual void loadAsset(const nlohmann::json & assetFile) override;

public:

    std::vector<int> _animationFrames;
    std::vector<float> _animationSpeeds;
    sf::Vector2f _rectangleSize;
    std::string _texture;

};

}