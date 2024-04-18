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

	sf::Text text;

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
		writer.EndObject();
	}

	void deserialize(const rapidjson::Value& value) override {
		name = value["name"].GetString();
		loadFromAssetManager();
		message = value["message"].GetString();
		style = value["style"].GetUint();
		character_size = value["characterSize"].GetInt();
		text_color = sf::Color(value["textColorR"].GetUint(), value["textColorG"].GetUint(), value["textColorB"].GetUint(), value["textColorA"].GetUint());
	}
};