#pragma once
#include "Component.h"

class CHealth : public Component {
public:
	DECLARE_COMPONENT_NAME("Health");
	float healthTotal_ = 100.0;
	float currentHealth_ = 100.0;
	float percentage_ = 1;
	bool drawHealth_ = true;
	bool followEntity = true;
	sf::Sprite frontHealthBar_;
	sf::Sprite backHealthBar_;
	sf::IntRect originalTextureBounds_; // Used to store the texture bounds of the frontHealthBar

	Vec2 healthBarScale_ = Vec2(.5,.5); 
	Vec2 healthBarPosition_ = Vec2(50, 50); //If th user wants the healthbar to not follow the Entity

	Vec2 healthBarOffset_ = Vec2(0, -50);
	
	bool respawnCharacter_ = false;
	Vec2 respawnPosition_ = Vec2(0, 0); // Respawn the character

	CHealth();
	CHealth(float healthTotal, float currentHealth);
	void Update();
};
	