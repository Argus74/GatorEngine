#include "CHealth.h"
#include "../AssetManager.h"


CHealth::CHealth() {
	auto& assetManager = AssetManager::GetInstance();
	front_health_bar.setTexture(assetManager.GetTexturePrivate("HealthBarFull")); //For now hard code the assets used for health bar
	back_health_bar.setTexture(assetManager.GetTexturePrivate("HealthBarEmpty"));
	original_texture_bounds = front_health_bar.getTextureRect();
}

CHealth::CHealth(float healthTotal, float currentHealth) {
	auto& assetManager = AssetManager::GetInstance();
	front_health_bar.setTexture(assetManager.GetTexturePrivate("HealthBarFull")); //For now hard code the assets used for health bar
	back_health_bar.setTexture(assetManager.GetTexturePrivate("HealthBarEmpty"));
	original_texture_bounds = front_health_bar.getTextureRect();
	health_total = healthTotal;
	current_health = currentHealth;
}

void CHealth::Update() { 
	if (current_health > health_total) {
		current_health = health_total;
	}
	float percentage = current_health / health_total;

	// Calculate the visible width based on the percentage
	int visibleWidth = static_cast<int>(original_texture_bounds.width * percentage);

	// Set the texture rectangle of the sprite to only show a portion
	front_health_bar.setTextureRect(sf::IntRect(original_texture_bounds.left, original_texture_bounds.top, visibleWidth, original_texture_bounds.height));

	sf::Color color = AssetManager::LerpColor(sf::Color::Red, sf::Color::Green, percentage);
	front_health_bar.setColor(color);

}

bool CHealth::UpdateHealth(float dmg) { // Returns True if entity should be dead, returns false if not
	current_health += dmg;

	if (current_health <= 0) {
		current_health = health_total;

		return true;
	}
	else
		return false;
}




