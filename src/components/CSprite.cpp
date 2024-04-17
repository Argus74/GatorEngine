#include "CSprite.h"
#include "../AssetManager.h"


CSprite::CSprite() {
	loadFromAssetManager();
}

CSprite::CSprite(const std::string& name) : name_(name) {
	loadFromAssetManager();
}

bool CSprite::loadFromAssetManager() {
	auto& assetManager = AssetManager::GetInstance();
	texture_ = assetManager.GetTexture(name_);
	sprite_.setTexture(texture_);
	sprite_.setOrigin(texture_.getSize().x / 2, texture_.getSize().y / 2);
	return true;
}

bool CSprite::loadFromAssetManager(const std::string& textureName) {
	auto& assetManager = AssetManager::GetInstance();
	texture_ = assetManager.GetTexture(textureName);
	sprite_.setTexture(texture_);
	sprite_.setOrigin(texture_.getSize().x / 2, texture_.getSize().y / 2);
	return true;
}