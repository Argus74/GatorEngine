#pragma once
#include "Component.h"
class CSprite : public Component {
public:
	std::string name_ = "DefaultSprite";
	sf::Texture texture_;
	bool drawSprite_ = true;
	sf::Sprite sprite_;
	CSprite();
	CSprite(const std::string& name);

	bool loadFromAssetManager();
	bool loadFromAssetManager(const std::string& textureName);
	void setPosition(float x, float y);
	void setPosition(Vec2 pos);
	void setScale(float xScale, float yScale);
	void setScale(Vec2 pos);
	void setTexturePortion(sf::Rect<int> portion);
};