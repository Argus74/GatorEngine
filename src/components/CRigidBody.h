#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
class CRigidBody : public Component {
public:
	DECLARE_COMPONENT_NAME("Rigid Body");
	bool static_body;
	b2Body* body;
	b2Fixture* fixture;
	CRigidBody() : static_body(true) {}
	CRigidBody(bool flag, b2Body* b, b2Fixture* f) : static_body(flag), body(b), fixture(f) {}

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