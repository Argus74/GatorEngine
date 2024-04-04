#include "Animation.h"
#include <cmath>

Animation::Animation() {

};

Animation::Animation(const std::string& name, const sf::Texture& texture)
	: Animation(name, texture, 1, 0)
{

};

Animation::Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, size_t animationSpeed) 
: name(name),
sprite(texture),
frame_count(frameCount),
current_frame(0),
speed(animationSpeed)
{
    size = Vec2((float)texture.getSize().x / frameCount, (float)texture.getSize().y);
    sprite.setTextureRect(sf::IntRect(std::floor(current_frame) * size.x, 0, size.x, size.y)); //Setting the texture of our Rectangle to be the first frame/current
    sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
};

void Animation::Update() {
    // Increase the current frame based on the speed
    current_frame += speed;

    // Check if the animation has reached the end
    if (current_frame >= frame_count) {
        current_frame = 0;
        reached_end = true;
    }

    // Calculating the position of the current frame within the texture
    float framePosition = current_frame * size.x;

    // Update the texture rectangle to display the current frame
    sprite.setTextureRect(sf::IntRect(framePosition, 0, size.x, size.y));
    sprite.setOrigin(size.x / 2, size.y / 2);
};

const Vec2& Animation::GetSize() const
{
	return size;
}


const std::string& Animation::GetName() const
{
	return name;
}

sf::Sprite& Animation::GetSprite()
{
	return sprite;
}

bool Animation::HasEnded() const
{
	// TODO: detect when animation has ended (last frame was played) and return true
	return false;
}