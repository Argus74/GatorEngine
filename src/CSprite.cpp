#include "CSprite.h"

CSprite::CSprite()
{
	sf::Texture white_box;
	white_box.loadFromFile("assets/Hello.png");

	sprite_.setTexture(white_box);
}

void CSprite::SetTexture(sf::Texture texture_to_set)
{
	sprite_.setTexture(texture_to_set);
}

void CSprite::SetPosition(sf::Vector2f position)
{
	sprite_.setPosition(position);
}








