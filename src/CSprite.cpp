#include "Components.h"
#include "AssetManager.h"


CSprite::CSprite() {
	loadFromAssetManager();
}

CSprite::CSprite(const std::string& name) : name_(name) {
	loadFromAssetManager();
}

bool CSprite::loadFromAssetManager() {
	auto& assetManager = AssetManager::GetInstance();
	texture_ = assetManager.GetTexture(name_);
	return true;
}

bool CSprite::loadFromAssetManager(const std::string& textureName) {
	auto& assetManager = AssetManager::GetInstance();
	texture_ = assetManager.GetTexture(textureName);
	return true;
}

/*void CSprite::setPosition(float x, float y) {
	sprite_.setPosition(x, y);
}

void CSprite::setPosition(Vec2 pos) {
	float x = pos.x;
	float y = pos.y;
	std::cout << "Pos: " << x << ", " << y << std::endl;
	sprite_.setPosition(x, y);
}


void CSprite::setScale(float xScale, float yScale) {
	sprite_.setScale(xScale, yScale);
}

void CSprite::setScale(Vec2 scale) {
	float x = scale.x;
	float y = scale.y;
	std::cout << "Scale: " << x << ", " << y << std::endl;
	sprite_.setScale(x, y);
} */
