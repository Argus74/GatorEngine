#include "CAnimation.h"

#include "../AssetManager.h"

CAnimation::CAnimation() {
    loadFromAssetManager();
}

CAnimation::CAnimation(std::string name) : name(name) {
    loadFromAssetManager();
}

void CAnimation::loadFromAssetManager() {
	auto& assetManager = AssetManager::GetInstance();
	animation = assetManager.GetAnimation(name);
	animation_speed = 1;
}

void CAnimation::setAnimation(const Animation& newAnimation) {
    animation = newAnimation;
}

void CAnimation::changeSpeed() {
    animation.speed = animation_speed;
}

void CAnimation::update() {
    animation.Update();
}