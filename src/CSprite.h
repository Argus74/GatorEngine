#pragma once
#include "SFML/Graphics.hpp"

class CSprite {
public:
	sf::Sprite sprite_;

	CSprite();
	void SetTexture(sf::Texture texture_to_set);
	void SetPosition(sf::Vector2f position);

};