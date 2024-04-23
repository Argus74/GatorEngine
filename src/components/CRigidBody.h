/**
 * File: CRigidBody.h
 *
 * The CRigidBody class is a component for adding physical properties to entities in a game engine.
 *
 * This class allows entities to interact physically with other entities in the game world by providing them with a physical body
 * and fixtures that can be managed by a physics engine, specifically Box2D. It supports dynamic adjustment of physical properties
 * such as density, friction, and whether the body is static or dynamic. 
 */

#pragma once

#include <Box2D/Box2D.h>

#include "Component.h"

class CRigidBody : public Component {
 public:
    DECLARE_COMPONENT_NAME("Rigid Body");
    bool static_body;
    float density = 1.0f;
    float friction = 0.3f;
    b2Body* body;
    b2Fixture* fixture;
    b2Fixture* sensor_fixture;
    CRigidBody() : static_body(true) {}
    CRigidBody(bool flag, b2Body* b, b2Fixture* f, b2Fixture* sensorF)
        : static_body(flag), body(b), fixture(f), sensor_fixture(sensorF) {}

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("staticBody");
        writer.Bool(static_body);
        writer.EndObject();
    }
    void deserialize(const rapidjson::Value& value) override {
        static_body = value["staticBody"].GetBool();
    }
};