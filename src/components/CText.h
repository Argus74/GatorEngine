#pragma once
#include "Component.h"

class CText : public Component {
public:
	DECLARE_COMPONENT_NAME("Text");
	std::string name_ = "MontserratBlack";
	sf::Font font_;
	std::string message_ = "Default";
	unsigned int style_ = sf::Text::Regular;
	int characterSize_ = 24;
	sf::Color textColor_ = sf::Color::Black;
	float counter_ = 0;
	bool isCounter_ = false;

	sf::Text text_;

	CText();
	CText(const std::string& name);

	bool loadFromAssetManager();
	bool loadFromAssetManager(const std::string& textureName);


};