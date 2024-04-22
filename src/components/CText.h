/**
 * File: CText.h
 *
 * Manages the text rendering capabilities for entities within the game engine.
 *
 * This component class provides the functionality to attach textual display features to game entities. It allows
 * for setting text properties such as font, message content, styling (e.g., bold, italic), character size, and color.
 * The `CText` component utilizes SFML's Text and Font classes to manage and render text.
 */
#pragma once

#include "Component.h"

class CText : public Component {
 public:
    DECLARE_COMPONENT_NAME("Text");
    std::string name = "MontserratBlack";
    sf::Font font;
    std::string message = "Default";
    unsigned int style = sf::Text::Regular;
    int character_size = 24;
    sf::Color text_color = sf::Color::Black;
    float counter = 0;
    bool is_counter = false;

    sf::Text text;

    void reset() { counter = 0; }

    CText();
    CText(const std::string& name);

    bool loadFromAssetManager();
    bool loadFromAssetManager(const std::string& textureName);

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("name");
        writer.String(name.c_str());
        writer.Key("message");
        writer.String(message.c_str());
        writer.Key("style");
        writer.Uint(style);
        writer.Key("characterSize");
        writer.Int(character_size);
        writer.Key("textColorR");
        writer.Uint(text_color.r);
        writer.Key("textColorG");
        writer.Uint(text_color.g);
        writer.Key("textColorB");
        writer.Uint(text_color.b);
        writer.Key("textColorA");
        writer.Uint(text_color.a);
        writer.Key("counter");
        writer.Double(counter);
        writer.Key("isCounter");
        writer.Bool(is_counter);
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        if (value.HasMember("name") && value["name"].IsString()) {
            name = value["name"].GetString();
            loadFromAssetManager();
        }

        if (value.HasMember("message") && value["message"].IsString()) {
            message = value["message"].GetString();
        }

        if (value.HasMember("style") && value["style"].IsUint()) {
            style = value["style"].GetUint();
        }

        if (value.HasMember("characterSize") && value["characterSize"].IsInt()) {
            character_size = value["characterSize"].GetInt();
        }

        if (value.HasMember("textColorR") && value["textColorR"].IsUint() &&
            value.HasMember("textColorG") && value["textColorG"].IsUint() &&
            value.HasMember("textColorB") && value["textColorB"].IsUint() &&
            value.HasMember("textColorA") && value["textColorA"].IsUint()) {
            text_color = sf::Color(value["textColorR"].GetUint(), value["textColorG"].GetUint(),
                                   value["textColorB"].GetUint(), value["textColorA"].GetUint());
        }

        if (value.HasMember("counter") && value["counter"].IsDouble()) {
            counter = static_cast<float>(value["counter"].GetDouble());
        }

        if (value.HasMember("isCounter") && value["isCounter"].IsBool()) {
            is_counter = value["isCounter"].GetBool();
        }
    }
};