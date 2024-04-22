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

class Animation {
 public:
    Animation();
    Animation(const std::string& name, const sf::Texture& texture);
    Animation(const std::string& name, const sf::Texture& texture, size_t frameCount,
              size_t animationSpeed);
    sf::Sprite sprite;         // the sprite to display the animation
    size_t frame_count = 1;    // total number of frames of animation
    size_t current_frame = 0;  // the current frame of animation being played
    float speed = 0.0;         // the speed to play this animation
    Vec2 size = {1, 1};        // size of the animation frame
    std::string name = "none";
    bool reached_end = false;
    float frame_remainder = 0.0f;

    void Update();
    bool HasEnded() const;
    const std::string& GetName() const;
    const Vec2& GetSize() const;
    sf::Sprite& GetSprite();
};