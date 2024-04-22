/**
 * File: CCollectable.h
 *
 * The CCollectable class manages the properties and behaviors of collectable items within the game.
 *
 * This component allows for the customization of various collectable properties such as health benefits, point values, and interaction effects
 * like disappearing on touch. It is versatile for use in various game scenarios where items can be collected by players or other entities.
 */

#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "Component.h"

class CCollectable : public Component {
 public:
    DECLARE_COMPONENT_NAME("Collectable");

    bool is_health = false;  // Determines if the collectable adds points or not
    float points_to_add = 25.0;
    bool disappear_on_touch = true;
    std::string text_entity_name = "";

    bool touched = false;  // If the collectable is touched, this is the flag

    bool reset() {
        if (touched && disappear_on_touch)
            return true;  // Respawn the entity
        else
            return false;
    }

    CCollectable() {}

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();

        writer.Key("is_health");
        writer.Bool(is_health);

        writer.Key("points_to_add");
        writer.Double(points_to_add);

        writer.Key("disappear_on_touch");
        writer.Bool(disappear_on_touch);

        writer.Key("text_entity_name");
        writer.String(text_entity_name.c_str());

        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        if (value.HasMember("is_health") && value["is_health"].IsBool()) {
            is_health = value["is_health"].GetBool();
        }

        if (value.HasMember("points_to_add") && value["points_to_add"].IsDouble()) {
            points_to_add = static_cast<float>(value["points_to_add"].GetDouble());
        }

        if (value.HasMember("disappear_on_touch") && value["disappear_on_touch"].IsBool()) {
            disappear_on_touch = value["disappear_on_touch"].GetBool();
        }

        if (value.HasMember("text_entity_name") && value["text_entity_name"].IsString()) {
            text_entity_name = value["text_entity_name"].GetString();
        }
    }
};

#endif  // COLLECTABLE_H