#pragma once
#include "Box2D/Box2D.h"
#include <vector>
#include <map>
#include "Entity.h"
#include "Vec2.h"

class GatorPhysics : b2ContactListener {

private:
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	float timeStep = 1.0f / 60.0f;
	float scale_ = 30.0f;
	GatorPhysics();
	b2Vec2 gravity_ = b2Vec2(0, -10 / scale_);
	b2World world_ = b2World(gravity_);
	std::map<Entity*, b2Body*> entity_to_bodies_;
public:

	static GatorPhysics& GetInstance();
	b2World* getWorld();
	float getScale();
	std::map<Entity*, b2Body*>& GetEntityToBodies();
	void update();
	void setGravity(Vec2 gravity);
	void createBody(Entity* entity, bool is_static);
	void destroyBody(Entity* entity);
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};