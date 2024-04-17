#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
class CRigidBody : public Component {
public:
	DECLARE_COMPONENT_NAME("Rigid Body");
	bool staticBody;
	float density = 1.0f;
	float friction = 0.3f;
	b2Body* body;
	b2Fixture* fixture;
	b2Fixture* sensor_fixture;
	CRigidBody() : staticBody(true) {}
	CRigidBody(bool flag, b2Body* b, b2Fixture* f, b2Fixture* sensor_f) : staticBody(flag), body(b), fixture(f), sensor_fixture(sensor_f) {}
};