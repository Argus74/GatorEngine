#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
class CRigidBody : public Component {
public:
	bool staticBody;
	b2Body* body;
	b2Fixture* fixture;
	CRigidBody() : Component("Rigid Body"), staticBody(true) {}
	CRigidBody(bool flag, b2Body* b, b2Fixture* f) : Component("Rigid Body"), staticBody(flag), body(b), fixture(f) {}
};