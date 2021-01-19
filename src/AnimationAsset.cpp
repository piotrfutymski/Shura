#include "AnimationAsset.h"

namespace Didax
{

void AnimationAsset::loadAsset(const nlohmann::json & assetFile)
{
	_texture = assetFile["texture"];
    _rectangleSize.x = assetFile["sizeX"];
    _rectangleSize.y = assetFile["sizeY"];
    for(auto & anim : assetFile["animations"])
    {
        _animationFrames.push_back(anim["frames"]);
        _animationSpeeds.push_back(anim["speed"]);
    }
}   

}