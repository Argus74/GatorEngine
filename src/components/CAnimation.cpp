#include "CAnimation.h"
#include "../AssetManager.h"

CAnimation::CAnimation() {}

CAnimation::CAnimation(std::string name) : name(name) { 
	auto& assetManager = AssetManager::GetInstance();
	animation = assetManager.GetAnimation(name);
	animation_speed = animation.speed;
}

void CAnimation::setAnimation(const Animation& newAnimation) {
	animation = newAnimation;
}

void CAnimation::changeSpeed() {
	animation_speed = animation.speed;
}

void CAnimation::update() {
	animation.Update();

}