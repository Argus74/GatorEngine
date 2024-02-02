#pragma once
#include "imgui-SFML.h"
#include "SFML/Graphics/Transform.hpp"
class CTransform {

public:

	sf::Transform transform_;
	//Constructor does nothing (besides initializing a default transform)
	//with no position or rotation
	CTransform();
	//Translates a transform by the offset
	void Translate(sf::Vector2f offset);
	//Modifies only the position portion of the transform
	void SetPosition(sf::Vector2f position);
	//Gets the position portion of the transform
	sf::Vector2f GetPosition();
};