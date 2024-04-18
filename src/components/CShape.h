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

	void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
		writer.StartObject();
		writer.Key("type");
		writer.String(type.c_str());
		writer.Key("r");
		writer.Int(color.r);
		writer.Key("g");
		writer.Int(color.g);
		writer.Key("b");
		writer.Int(color.b);
		writer.Key("a");
		writer.Int(color.a);
		writer.EndObject();
	}

    void deserialize(const rapidjson::Value& value) override {
		type = value["type"].GetString();
		color = sf::Color(value["r"].GetInt(), value["g"].GetInt(), value["b"].GetInt(), value["a"].GetInt());
	}
};