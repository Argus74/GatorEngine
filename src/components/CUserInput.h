/*
	File: CUserInput.h
	The cUserInput component allows an entity to register Actions based on user inputs.
	These user inputs include key presses and mouse clicks, and are mapped to pre-defined
	Actions. The list of available Actions is defined in the ActionBus class.
*/

#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <SFML/Window.hpp>

#include "../ActionBus.h"
#include "Component.h"

class CUserInput : public Component {
public:
	DECLARE_COMPONENT_NAME("User Input");
	std::map<sf::Mouse::Button, Action> mouse_map;
	std::map<sf::Keyboard::Key, Action> key_map;
	CUserInput() {}
};

#endif // USER_INPUT_H