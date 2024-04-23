/* File: CAnimation.h
 *
 * The CAnimation class is a component used to manage and control animations for entities in a game engine.
 *
 * This class handles the animation states and properties such as animation speed and active animation frames. It allows
 * entities to display complex animations with multiple frames, supporting both playback and on-the-fly adjustments such as speed changes.
 * The component can be extended to include various animation states (like jumping or running) based on the entity's current actions or conditions.
 */

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
    bool disappear;
    // If in the editor they want to play test their animation
    bool play_animation = false;
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
