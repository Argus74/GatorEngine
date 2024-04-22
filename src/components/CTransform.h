/*
 * File: CTransform.h
 *
 * This file defines the CTransform class, which is a core component in a
 * game engine's entity-component system architecture. It primarily manages
 * spatial properties of game entities, including position, scale, orientation,
 * and velocity.
 */
#pragma once

#include "SFML/Graphics/Transform.hpp"
#include "imgui-SFML.h"

#include "../Vec2.h"
#include "Component.h"

class CTransform : public Component {
 public:
    DECLARE_COMPONENT_NAME("Transform");
    Vec2 origin, position, scale, velocity;
    float angle;
    CTransform() : origin(Vec2(0, 0)), position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
    CTransform(Vec2 origin) : origin(origin), position(origin), scale(Vec2(1, 1)), angle(0) {}
    CTransform(const Vec2& origin, const Vec2& scl, float ang)
        : origin(origin), position(origin), scale(scl), angle(ang) {}

    void resetPosition() { position = origin; };

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("originX");
        writer.Double(origin.x);
        writer.Key("originY");
        writer.Double(origin.y);
        writer.Key("scaleX");
        writer.Double(scale.x);
        writer.Key("scaleY");
        writer.Double(scale.y);
        writer.Key("angle");
        writer.Double(angle);
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        origin = position = Vec2(value["originX"].GetDouble(), value["originY"].GetDouble());
        scale = Vec2(value["scaleX"].GetDouble(), value["scaleY"].GetDouble());
        angle = value["angle"].GetDouble();
    }
};