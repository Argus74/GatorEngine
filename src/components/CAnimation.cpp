#include "CAnimation.h"

#include "../AssetManager.h"

CAnimation::CAnimation() {}

CAnimation::CAnimation(std::string name) : name(name) {
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