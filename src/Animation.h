/**
 * Animation.h
 * Defines the Animation class for animating sprites in Gator Engine.
 *
 * The Animation class is responsible for handling sprite-based animations. It
 * provides functionality to update animation frames based on the speed of the
 * animation and the number of frames. An instance of Animation contains a sprite,
 * texture, and properties to control the frame rate and track the current frame.
 * This class can be used to animate any sprite, from characters to environment
 * elements, by updating the sprite's texture rectangle to the correct frame during
 * each update cycle.
 */

#include <cmath>
#include <iostream>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "Vec2.h"
#include "util/Serializable.h"


class Animation : public Serializable {
 public:
    Animation();
    Animation(const std::string& name, const std::string& textureName, const sf::Texture& texture);
    Animation(const std::string& name, const std::string& textureName, const sf::Texture& texture, size_t frameCount,
              size_t animationSpeed);
    sf::Sprite sprite;         // the sprite to display the animation
    size_t frame_count = 1;    // total number of frames of animation
    size_t current_frame = 0;  // the current frame of animation being played
    float speed = 0.0;         // the speed to play this animation
    Vec2 size = {1, 1};        // size of the animation frame
    std::string name = "none";
    bool reached_end = false;
    float frame_remainder = 0.0f;
    std::string textureName = "";

    void Update();
    bool HasEnded() const;
    const std::string& GetName() const;
    const Vec2& GetSize() const;
    sf::Sprite& GetSprite();

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("name");
        writer.String(name.c_str());
        writer.Key("speed");
        writer.Double(speed);
        writer.Key("frameCount");
        writer.Uint64(frame_count);
        writer.Key("textureName");
        writer.String(textureName.c_str());
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        if (value.HasMember("name"))
            name = value["name"].GetString();
        if (value.HasMember("speed"))
            speed = value["speed"].GetDouble();
        if (value.HasMember("frameCount"))
            frame_count = value["frameCount"].GetUint64();
        if (value.HasMember("textureName"))
            textureName = value["textureName"].GetString();
        
        
    }
};