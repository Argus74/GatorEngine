#include "Animation.h"
#include <cmath>

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
    sprite_.setTextureRect(sf::IntRect(std::floor(currentFrame_) * size_.x, 0, size_.x, size_.y)); //Setting the texture of our Rectangle to be the first frame/current
    sprite_.setOrigin(size_.x / 2.0f, size_.y / 2.0f);
};

void Animation::Update() {
    
    
    frameRemainder_ += speed_;  
    currentFrame_ += frameRemainder_ / 1; //Any whole frames are added to currentFrame while the rest go back to frameRemainder
    

    while (frameRemainder_ >= 1.0f) { //Mod % 1 essentially, keep tracking of frame left overs
        frameRemainder_ -= 1.0f;
    }

    // Check if the animation has reached the end
    if (currentFrame_ >= frameCount_) {
        currentFrame_ = 0;
        reachedEnd_ = true;
    }

    // Calculating the position of the current frame within the texture
    float framePosition = currentFrame_ * size_.x;

    // Update the texture rectangle to display the current frame
    sprite_.setTextureRect(sf::IntRect(framePosition, 0, size_.x, size_.y));
    sprite_.setOrigin(size_.x / 2, size_.y / 2);
};

const Vec2& Animation::GetSize() const
{
    return size_;
}


const std::string& Animation::GetName() const
{
    return name_;
}

sf::Sprite& Animation::GetSprite()
{
    return sprite_;
}

bool Animation::HasEnded() const
{
    // TODO: detect when animation has ended (last frame was played) and return true
    return false;
}