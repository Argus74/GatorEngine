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

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("name");
        writer.String(name.c_str());
        writer.Key("drawSprite");
        writer.Bool(draw_sprite);
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        name = value["name"].GetString();
        loadFromAssetManager();
        draw_sprite = value["drawSprite"].GetBool();
    }
};