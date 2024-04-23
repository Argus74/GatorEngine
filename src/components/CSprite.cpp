#include "CSprite.h"

#include "../AssetManager.h"

CSprite::CSprite() {}

CSprite::CSprite(const std::string& name) : name(name) {
    loadFromAssetManager();
}

bool CSprite::loadFromAssetManager() {
    auto& assetManager = AssetManager::GetInstance();
    texture = assetManager.GetTexture(name);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    portion = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
    return true;
}

bool CSprite::loadFromAssetManager(const std::string& textureName) {
    auto& assetManager = AssetManager::GetInstance();
    texture = assetManager.GetTexture(textureName);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    portion = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
    return true;
}