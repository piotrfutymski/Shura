
#include "Asset.h"

namespace Didax
{

Asset::Asset(const std::string & name)
{
	_name = name;
}

Asset::~Asset()
{
}

const std::string & Asset::getName()const
{
	return _name;
}

}