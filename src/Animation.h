#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>
#include "Vec2.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Animation {
public:
	Animation();
	Animation(const std::string& name, const sf::Texture &texture);
	Animation(const std::string& name, const sf::Texture &texture, size_t frameCount, size_t animationSpeed);
	sf::Sprite sprite;		// the sprite to display the animation
	size_t frame_count = 1;	// total number of frames of animation
	size_t current_frame = 0;	// the current frame of animation being played
	float speed = 0.0;		// the speed to play this animation
	Vec2 size = { 1, 1 };	// size of the animation frame
	std::string	name = "none";
	bool reached_end = false;
	float frame_remainder = 0.0f;

	void Update();
	bool HasEnded() const;
	const std::string& GetName() const;
	const Vec2& GetSize() const;
	sf::Sprite& GetSprite();
};