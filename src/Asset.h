#pragma once


#include "nlohmann/json.hpp"
#include <map>

namespace Didax
{

class Asset
{
public:

	Asset(const std::string & name);
	~Asset();


	virtual void loadAsset(const nlohmann::json & assetFile) = 0;

	const std::string & getName()const;

protected:

	std::string _name;

};

}