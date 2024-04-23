/*
 * File: GatorPhysics.h
 *
 * This file defines the GatorPhysics class, which encapsulates the physics 
 * engine functionality for a game using the Box2D library. GatorPhysics is
 * responsible for managing physical interactions between game entities, 
 * applying gravity, handling collisions, and integrating physical responses.
 */

#pragma once
#include <map>
#include <vector>

#include "Box2D/Box2D.h"

#include "Entity.h"
#include "Vec2.h"

class GatorPhysics : b2ContactListener {
 private:
    int32 velocity_iterations_ = 6;
    int32 position_iterations_ = 2;
    float time_step_ = 1.0f / 60.0f;
    float scale_ = 30.0f;
    GatorPhysics();
    b2Vec2 gravity_ = b2Vec2(0, -10);
    b2World world_ = b2World(gravity_);
    std::map<Entity*, b2Body*> entity_to_bodies_;

 public:
    static GatorPhysics& GetInstance();
    b2World* getWorld();
    float getScale();
    std::map<Entity*, b2Body*>& GetEntityToBodies();
    void clearBodies();
    void update();
    void setGravity(Vec2 gravity);
    void createBody(Entity* entity, bool isStatic);
    void destroyBody(Entity* entity);
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
    void ResetWorld();
};