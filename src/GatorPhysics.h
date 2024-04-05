#pragma once
#include "Box2D/Box2D.h"
#include <vector>
#include <map>
#include "Entity.h"
#include "Vec2.h"

class GatorPhysics {

private:
		int32 velocity_iterations_ = 6;
		int32 position_iterations_ = 2;
		float time_step_ = 1.0f / 60.0f;
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
		void createBody(Entity* entity, bool isStatic);
		void destroyBody(Entity* entity);
};