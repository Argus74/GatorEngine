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
	sf::Sprite sprite_;		// the sprite to display the animation
	size_t		frameCount_ = 1;	// total number of frames of animation
	size_t		currentFrame_ = 0;	// the current frame of animation being played
	size_t		speed_ = 0;		// the speed to play this animation
	Vec2		size_ = { 1, 1 };	// size of the animation frame
	std::string	name_ = "none";
	bool reachedEnd_ = false;


	void Update();
	bool hasEnded() const;
	const std::string& getName() const;
	const Vec2& getSize() const;
	sf::Sprite& getSprite();
};