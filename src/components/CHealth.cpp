#include "CHealth.h"
#include "../AssetManager.h"


CHealth::CHealth() {
	auto& assetManager = AssetManager::GetInstance();
	frontHealthBar_.setTexture(assetManager.GetTexturePrivate("HealthBarFull")); //For now hard code the assets used for health bar
	backHealthBar_.setTexture(assetManager.GetTexturePrivate("HealthBarEmpty"));
	originalTextureBounds_ = frontHealthBar_.getTextureRect();
}

CHealth::CHealth(float healthTotal, float currentHealth) {
	auto& assetManager = AssetManager::GetInstance();
	frontHealthBar_.setTexture(assetManager.GetTexturePrivate("HealthBarFull")); //For now hard code the assets used for health bar
	backHealthBar_.setTexture(assetManager.GetTexturePrivate("HealthBarEmpty"));
	originalTextureBounds_ = frontHealthBar_.getTextureRect();
	healthTotal_ = healthTotal;
	currentHealth_ = currentHealth;
}

void CHealth::Update() { 
	if (currentHealth_ > healthTotal_) {
		currentHealth_ = healthTotal_;
	}
	float percentage = currentHealth_ / healthTotal_;

	// Calculate the visible width based on the percentage
	int visibleWidth = static_cast<int>(originalTextureBounds_.width * percentage);

	// Set the texture rectangle of the sprite to only show a portion
	frontHealthBar_.setTextureRect(sf::IntRect(originalTextureBounds_.left, originalTextureBounds_.top, visibleWidth, originalTextureBounds_.height));

	sf::Color color = AssetManager::LerpColor(sf::Color::Red, sf::Color::Green, percentage);
	frontHealthBar_.setColor(color);

}

bool CHealth::UpdateHealth(float dmg) { // Returns True if entity should be dead, returns false if not
	currentHealth_ += dmg;

	if (currentHealth_ <= 0) {
		currentHealth_ = healthTotal_; 
		return true;
	}
	else
		return false;
}




