#pragma once
#include "Component.h"

class CBackground : public Component {
public:
	DECLARE_COMPONENT_NAME("Background");
	sf::Color color;
	bool use_texture;
	sf::Sprite sprite;
	CBackground() : color(sf::Color::Blue), use_texture(false) {
		//default sprite somehow
	}
};