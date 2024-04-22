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

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("mouseMap");
        writer.StartObject();
        for (const auto& pair : mouse_map) {
            writer.Key(std::to_string(pair.first).c_str());
            writer.Int(pair.second);
        }
        writer.EndObject();

        writer.Key("keyMap");
        writer.StartObject();
        for (const auto& pair : key_map) {
            writer.Key(std::to_string(pair.first).c_str());
            writer.Int(pair.second);
        }
        writer.EndObject();

        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        for (auto it = value["mouseMap"].MemberBegin(); it != value["mouseMap"].MemberEnd(); ++it) {
            if (it->value.IsInt()) {
                mouse_map[static_cast<sf::Mouse::Button>(std::stoi(it->name.GetString()))] =
                    static_cast<Action>(it->value.GetInt());
            }
        }

        for (auto it = value["keyMap"].MemberBegin(); it != value["keyMap"].MemberEnd(); ++it) {
            if (it->value.IsInt()) {
                key_map[static_cast<sf::Keyboard::Key>(std::stoi(it->name.GetString()))] =
                    static_cast<Action>(it->value.GetInt());
            }
        }
    }
};

#endif  // USER_INPUT_H