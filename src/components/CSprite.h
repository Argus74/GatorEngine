#pragma once
#include "Component.h"
class CSprite : public Component {
public:
	DECLARE_COMPONENT_NAME("Sprite");
	std::string name = "DefaultSprite";
	sf::Texture texture;
	bool draw_sprite = true;
	sf::Sprite sprite;
	sf::Rect<int> portion;
	CSprite();
	CSprite(const std::string& name);

	bool loadFromAssetManager();
	bool loadFromAssetManager(const std::string& textureName);
	void setPosition(float x, float y);
	void setPosition(Vec2 pos);
	void setScale(float xScale, float yScale);
	void setScale(Vec2 pos);
	void setTexturePortion(sf::Rect<int> portion);

	void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
		writer.StartObject();
		writer.Key("name");
		writer.String(name.c_str());
		writer.Key("drawSprite");
		writer.Bool(draw_sprite);
		writer.Key("portionTop");
		writer.Int(portion.getPosition().y);
		writer.Key("portionLeft");
		writer.Int(portion.getPosition().x);
		writer.Key("portionX");
		writer.Int(portion.width);
		writer.Key("portionY");
		writer.Int(portion.height);
		writer.EndObject();
	}

    void deserialize(const rapidjson::Value& value) override {
		name = value["name"].GetString();
		loadFromAssetManager();
		draw_sprite = value["drawSprite"].GetBool();
		setTexturePortion(sf::IntRect(value["portionLeft"].GetInt(), value["portionTop"].GetInt(), value["portionX"].GetInt(), value["portionY"].GetInt()));
	}
};