#include "CHealth.h"
#include "../AssetManager.h"

CHealth::CHealth() {
	auto& assetManager = AssetManager::GetInstance();
	frontHealthBar_ = assetManager.GetTexturePrivate("HealthBarFull"); //For now hard code the assets used for health bar
	backHealthBar_ = assetManager.GetTexturePrivate("HealthBarEmpty");
}

CHealth::CHealth(float healthTotal, float currentHealth) {
	auto& assetManager = AssetManager::GetInstance();
	frontHealthBar_ = assetManager.GetTexturePrivate("HealthBarFull"); //For now hard code the assets used for health bar
	backHealthBar_ = assetManager.GetTexturePrivate("HealthBarEmpty");
	healthTotal_ = healthTotal;
	currentHealth_ = currentHealth;
};


