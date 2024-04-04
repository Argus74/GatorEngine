#include "CAnimation.h"
#include "../AssetManager.h"

CAnimation::CAnimation() { 
	auto& assetManager = AssetManager::GetInstance();
	animation = assetManager.GetAnimation(name);
	animation_speed = animation.speed;
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