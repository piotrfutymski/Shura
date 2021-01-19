#pragma once
#include <map>
#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

#include "Asset.h"
#include "TextureAsset.h"
#include "FontAsset.h"

namespace Didax
{

class AssetManager
{
public:

	using AssetHolder_t = std::map<std::string, std::unique_ptr<Asset>>;

	AssetManager();
	~AssetManager();

//Loading all assets
	static bool loadAllAssets(const nlohmann::json &settings);

//Loading one type of assets

	static bool loadAssetsFromFile(const std::string & filename);

	template<typename T>
	static typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type * createAsset(const std::string name)
	{
		_assets[name] = std::make_unique<T>(name);
		return static_cast<T *> (_assets[name].get());
	}

	template <typename T>
	static typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type * getAsset(const std::string name)
	{
		if (_assets.find(name) == _assets.end())
			return nullptr;
		else
			return static_cast<T*> (_assets[name].get());
	}

private:
	
	static AssetHolder_t _assets;

};

}