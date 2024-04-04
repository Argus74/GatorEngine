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
};