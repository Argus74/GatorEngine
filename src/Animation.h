#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>
#include "Vec2.h"
#include <iostream>

class Animation {
public:
	Animation();
	Animation(const std::string& name, const sf::Texture &texture);
	Animation(const std::string& name, const sf::Texture &texture, size_t frameCount, size_t animationSpeed);
	

	void Update();

	std::string name_ = "none";
	sf::Sprite sprite_; // Texture the animation gets frames from
	size_t frameCount_ = 1;
	size_t speed_ = 0; // Speed of animation
	size_t currentFrame_ = 0; //Current frame of the animation
	Vec2 size_ = { 1, 1 }; // (Width of a of the frame animation, height of a of the frame animation)
	bool reachedEnd_ = false;
	
};