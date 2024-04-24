/**
 * File: CCharacter.h
 *
 * The CCharacter class is a component that encapsulates character-specific behaviors and attributes,
 * such as movement speed and jump force, within a game engine.
 *
 * This class provides a straightforward way to manage and apply dynamic character behaviors such as running and jumping.
 * It includes attributes like speed and jump force which can be easily adjusted to customize character movement.
 * Additionally, it manages a grounded state to assist in determining if the character can jump or needs to fall.
 */

#pragma once

#include "../Vec2.h"
#include "Component.h"

class CCharacter : public Component {
 public:
    DECLARE_COMPONENT_NAME("Character");
    float speed;
    Vec2 jump_force;
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
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        speed = value["speed"].GetDouble();
        jump_force.x = value["jumpForceX"].GetDouble();
        jump_force.y = value["jumpForceY"].GetDouble();
    }
};