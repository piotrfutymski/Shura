#include "AssetManager.h"

namespace Didax
{

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

AssetManager::AssetHolder_t Didax::AssetManager::_assets = {};

bool Didax::AssetManager::loadAllAssets(const nlohmann::json &settings)
{
	auto assets = settings["assetsFileNames"];

	for (auto& assetCategory: assets)
		if (!loadAssetsFromFile(assetCategory)) return false;
		
	return true;
}

bool Didax::AssetManager::loadAssetsFromFile(const std::string & filename)
{
	std::ifstream stream(filename);
	nlohmann::json assetFile;
	if (stream.is_open())
	{
		stream >> assetFile;
	}
	else
	{
		std::cerr<<"Unable to load assets\n";
		return false;
	}

	for (auto& element : assetFile)
	{
		Asset * a;
		if (element["type"] == "font")
			a = createAsset<FontAsset>(element["name"]);
		else if (element["type"] == "texture")
			a = createAsset<TextureAsset>(element["name"]);
		else
			return false;
		try
		{
			a->loadAsset(element);
		}
		catch (const std::exception & e)
		{
			std::cerr<<e.what()<<"\n";
			return false;
		}

	}
	return true;
}

}