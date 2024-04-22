#pragma once

#include "Component.h"

class CHealth : public Component {
 public:
    DECLARE_COMPONENT_NAME("Health");
    float health_total = 100.0;
    float current_health = 100.0;
    float percentage = 1;
    bool draw_health = true;
    bool follow_entity = true;
    sf::Sprite front_health_bar;
    sf::Sprite back_health_bar;
    sf::IntRect original_texture_bounds;  // Used to store the texture bounds of the frontHealthBar

    Vec2 health_bar_scale = Vec2(.5, .5);
    //If th user wants the healthbar to not follow the Entity
    Vec2 health_bar_position = Vec2(50, 50);

    Vec2 health_bar_offset = Vec2(0, -50);

    bool respawn_character = false;
    Vec2 respawn_position = Vec2(0, 0);  // Respawn the character

    bool killed = false;

    bool reset() {
        current_health = health_total;
        if (killed)
            return true;
        return false;
    }

    CHealth();
    CHealth(float healthTotal, float currentHealth);
    void Update();
    bool UpdateHealth(float dmg);

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("healthTotal");
        writer.Double(health_total);
        writer.Key("currentHealth");
        writer.Double(current_health);
        writer.Key("drawHealth");
        writer.Bool(draw_health);
        if (draw_health) {
            writer.Key("followEntity");
            writer.Bool(follow_entity);
            if (follow_entity) {
                writer.Key("barOffsetX");
                writer.Double(health_bar_offset.x);
                writer.Key("barOffsetY");
                writer.Double(health_bar_offset.y);
            } else {
                writer.Key("barPositionX");
                writer.Double(health_bar_position.x);
                writer.Key("barPositionY");
                writer.Double(health_bar_position.y);
            }
            writer.Key("barScaleX");
            writer.Double(health_bar_scale.x);
            writer.Key("barScaleY");
            writer.Double(health_bar_scale.y);
        }
        writer.Key("respawnEntity");
        writer.Bool(respawn_character);
        if (respawn_character) {
            writer.Key("respawnPositionX");
            writer.Double(respawn_position.x);
            writer.Key("respawnPositionY");
            writer.Double(respawn_position.y);
        }
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        health_total = value["healthTotal"].GetDouble();
        current_health = value["currentHealth"].GetDouble();
        draw_health = value["drawHealth"].GetBool();

        if (draw_health) {
            follow_entity = value["followEntity"].GetBool();

            if (follow_entity) {
                health_bar_offset.x = value["barOffsetX"].GetDouble();
                health_bar_offset.y = value["barOffsetY"].GetDouble();
            } else {
                health_bar_position.x = value["barPositionX"].GetDouble();
                health_bar_position.y = value["barPositionY"].GetDouble();
            }

            health_bar_scale.x = value["barScaleX"].GetDouble();
            health_bar_scale.y = value["barScaleY"].GetDouble();
        }

        respawn_character = value["respawnEntity"].GetBool();

        if (respawn_character) {
            respawn_position.x = value["respawnPositionX"].GetDouble();
            respawn_position.y = value["respawnPositionY"].GetDouble();
        }
    }
};
