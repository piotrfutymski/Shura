#include "TextureAsset.h"

namespace Didax
{

void TextureAsset::loadAsset(const nlohmann::json & assetFile)
{
	sf::Image i;
	i.loadFromFile(_name);
	i.createMaskFromColor(sf::Color{ 255,255,255,255 });
	_texture.loadFromImage(i);
}

}