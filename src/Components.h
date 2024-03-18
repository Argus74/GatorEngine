#pragma once

#include "AssetManager.h"
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

class Component
{
public:
	bool has = false;
};

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

class CAnimation : public Component {
public:
	Animation animation_;
	sf::Sprite sprite_;
	std::string name_ = "DefaultAnimation";
	//std::string jumpAnimationName_ = "";           Later on if we decide we can make it easy to switch to a jump animation based off the characters state. Up to y'all
	float animationSpeed_;
	bool disappear_;
	//bool jumpAnimation;
	
	CAnimation();
	void changeSpeed();
	void update();
	
	//Will add a constructor that creates an animation within the component or we can decide for a dedicated place to make animations

	void setAnimation(const Animation& newAnimation);

};

