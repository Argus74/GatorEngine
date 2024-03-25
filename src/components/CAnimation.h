#pragma once
#include "Component.h"


class CAnimation : public Component {
public:
	DECLARE_COMPONENT_NAME("Animation");
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
