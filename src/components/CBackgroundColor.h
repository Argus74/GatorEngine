/*
* File:   CBackgroundColor.h
* 
* This component is used to determine the clear color of the program, which happens before
* any scene or editor rendering is done (see main.cpp and GameEngine::update()).
*/

#pragma once
#include "Component.h"

class CBackgroundColor : public Component {
public:
	DECLARE_COMPONENT_NAME("Background Color");
	sf::Color color;
	CBackgroundColor() : color(sf::Color::Blue) {}
};