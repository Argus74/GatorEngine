#pragma once

#include "../Vec2.h"
#include "Component.h"

class CCharacter : public Component {
 public:
    DECLARE_COMPONENT_NAME("Character");
    float speed;
    Vec2 jump_force;
    bool is_grounded = true;
    CCharacter() : speed(8.0f), jump_force(Vec2(0, 10)) {}
    CCharacter(float input_speed, Vec2 input_jump_force)
        : speed(input_speed), jump_force(input_jump_force) {}

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("speed");
        writer.Double(speed);
        writer.Key("jumpForceX");
        writer.Double(jump_force.x);
        writer.Key("jumpForceY");
        writer.Double(jump_force.y);
        writer.Key("isGrounded");
        writer.Bool(is_grounded);
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        speed = value["speed"].GetDouble();
        jump_force.x = value["jumpForceX"].GetDouble();
        jump_force.y = value["jumpForceY"].GetDouble();
        is_grounded = value["isGrounded"].GetBool();
    }
};