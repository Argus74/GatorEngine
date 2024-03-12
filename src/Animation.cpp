#include "Animation.h"

Animation::Animation() {

};

Animation::Animation(const std::string& name, const sf::Texture& texture)
	: Animation(name, texture, 1, 0)
{

};

Animation::Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, size_t animationSpeed) 
: name_(name),
sprite_(texture),
frameCount_(frameCount),
currentFrame_(0),
speed_(animationSpeed)
{
    size_ = Vec2((float)texture.getSize().x / frameCount, (float)texture.getSize().y);
    sprite_.setOrigin(size_.x / 2.0f, size_.y / 2.0f);
    sprite_.setTextureRect(sf::IntRect(std::floor(currentFrame_) * size_.x, 0, size_.x, size_.y)); //Setting the texture of our Rectangle to be the first frame/current
};

void Animation::Update() {
    // Increase the current frame based on the speed
    currentFrame_ += speed_;

    // Check if the animation has reached the end
    if (currentFrame_ >= frameCount_) {
        currentFrame_ = 0;
        reachedEnd_ = true;
    }

    // Calculating the position of the current frame within the texture
    float framePosition = currentFrame_ * size_.x;

    // Update the texture rectangle to display the current frame
    sprite_.setTextureRect(sf::IntRect(framePosition, 0, size_.x, size_.y));
};

