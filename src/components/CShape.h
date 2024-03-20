#pragma once
#include "SFML/Graphics.hpp"
#include "Component.h"

class CShape : public Component {
public:
	std::string type;
	sf::Color color;
	CShape() : Component("Shape"), type("Rectangle"), color(sf::Color::White) {}
	CShape(const std::string& t, const sf::Color& c) : Component("Shape"), type(t), color(c) {}
};