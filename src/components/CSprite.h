/**
 * File CSprite.h
 *
 * The CSprite class manages the graphical representation of game entities using sprites.
 *
 * This component allows entities to have visual representations through textures and sprites drawn on the screen.
 * It integrates with the AssetManager to manage texture resources, ensuring efficient memory and rendering performance.
 * The CSprite component is essential for rendering static or animated images attached to game entities, handling their visibility and portion of the texture to be displayed.
 */
#pragma once

#include "Component.h"

class CSprite : public Component {
 public:
    DECLARE_COMPONENT_NAME("Sprite");
    std::string name = "None";
    sf::Texture texture;
    bool draw_sprite = true;
    sf::Sprite sprite;
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