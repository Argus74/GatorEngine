#pragma once
#include "Component.h"

class CHealth : public Component {
public:
	DECLARE_COMPONENT_NAME("Health");
	float healthTotal_ = 100;
	float currentHealth_ = 100;
	bool drawHealth_ = true;
	sf::Texture frontHealthBar_;
	sf::Texture backHealthBar_;
	Vec2 healthBarScale_ = Vec2(1, 1);
	Vec2 healthBarPosition_ = Vec2(50, 50);

	bool respawnCharacter = true;
	Vec2 respawnPosition = Vec2(0, 0); // Respawn the character

	CHealth();
	CHealth(float healthTotal, float currentHealth);

};
	