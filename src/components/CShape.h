#pragma once
#include "SFML/Graphics.hpp"
#include "Component.h"

class CShape : public Component {
public:
	DECLARE_COMPONENT_NAME("Shape");
	std::string type;
	sf::Color color;
	CShape() : type("Rectangle"), color(sf::Color::White) {}
	CShape(const std::string& t, const sf::Color& c) : type(t), color(c) {}
};