#include "FontAsset.h"

namespace Didax
{

void FontAsset::loadAsset(const nlohmann::json & assetFile)
{
	_font.loadFromFile(_name);

}

}