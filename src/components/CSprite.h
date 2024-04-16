#pragma once
#include "Component.h"
class CSprite : public Component {
public:
	DECLARE_COMPONENT_NAME("Sprite");
	std::string name_ = "DefaultSprite";
	sf::Texture texture_;
	bool drawSprite_ = true;
	sf::Sprite sprite_;
	CSprite();
	CSprite(const std::string& name);

	bool loadFromAssetManager();
	bool loadFromAssetManager(const std::string& textureName);
};