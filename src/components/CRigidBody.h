#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
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
	CRigidBody(bool flag, b2Body* b, b2Fixture* f, b2Fixture* sensorF) : static_body(flag), body(b), fixture(f), sensor_fixture(sensorF) {}

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