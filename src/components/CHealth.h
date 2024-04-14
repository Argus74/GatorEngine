#pragma once
#include "imgui-SFML.h"
#include "Component.h"

class CHealth : public Component {
public:
	DECLARE_COMPONENT_NAME("Health");
	float healthTotal_;
	float currentHealth_;
	bool drawHealth_;
	sf::Texture frontHealthBar_;
	sf::Texture backHealthBar_;
	Vec2 healthBarScale_;
	Vec2 healthBarPosition_;

	CHealth();
	CHealth(float healthTotal, float currentHealth)
		: healthTotal_(healthTotal), currentHealth_(currentHealth) {}

};
	