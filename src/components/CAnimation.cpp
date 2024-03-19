#include "CAnimation.h"
#include "../AssetManager.h"

CAnimation::CAnimation() { 
	auto& assetManager = AssetManager::GetInstance();
	animation_ = assetManager.GetAnimation(name_);
	animationSpeed_ = animation_.speed_;
}

void CAnimation::setAnimation(const Animation& newAnimation) {
	animation_ = newAnimation;
}

void CAnimation::changeSpeed() {
	animation_.speed_ = animationSpeed_;
}

void CAnimation::update() {
	animation_.Update();

}