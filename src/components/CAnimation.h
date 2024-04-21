#pragma once
#include "Component.h"


class CAnimation : public Component {
public:
	DECLARE_COMPONENT_NAME("Animation");
	Animation animation;
	sf::Sprite sprite;
	std::string name = "DefaultAnimation";
	//std::string jumpAnimationName_ = "";           Later on if we decide we can make it easy to switch to a jump animation based off the characters state. Up to y'all
	float animation_speed = 1;
	bool play_animation = false; // If in the editor they want to play test their animation
	bool disappear;
	//bool jumpAnimation;
	
	CAnimation();
	CAnimation(std::string name);
	void changeSpeed();
	void update();
	
	//Will add a constructor that creates an animation within the component or we can decide for a dedicated place to make animations

	void setAnimation(const Animation& newAnimation);

	void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
		writer.StartObject();
		writer.Key("name");
		writer.String(name.c_str());
		writer.EndObject();
	}

    void deserialize(const rapidjson::Value& value) override {
		name = value["name"].GetString();
		animation = AssetManager::GetInstance().GetAnimation(name);
		animation_speed = animation.speed;
	}
};
