#include "GatorPhysics.h"



GatorPhysics::GatorPhysics()
{

}

GatorPhysics& GatorPhysics::GetInstance()
{
	static GatorPhysics instance_;
	return instance_;
}

b2World* GatorPhysics::getWorld()
{
	return nullptr;
}

void GatorPhysics::update()
{
	for (auto node: entity_to_bodies_) {
		//If anything happened to the positions/rotation of the entities, update their physics bodies
		node.second->SetTransform(b2Vec2(node.first->cTransform->position.x, node.first->cTransform->position.y), node.first->cTransform->angle);
	}

	//Step the physics world
	world_.Step(timeStep, velocityIterations, positionIterations);

	for (auto node : entity_to_bodies_) {
		//Update the positions/rotation of the entities to match the physics bodies
		node.first->cTransform->position = Vec2(node.second->GetPosition().x, node.second->GetPosition().y);
	}
}

void GatorPhysics::setGravity(Vec2 gravity)
{
	gravity_ = b2Vec2(gravity.x, gravity.y);
}

void GatorPhysics::createBody(Entity* entity, bool is_static)
{
	b2BodyDef newBodyDef;
	b2PolygonShape newBox;
	newBox.SetAsBox(entity->getTransform()->scale.x, entity->getTransform()->scale.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &newBox;
	
	if (is_static)
	{
		newBodyDef.type = b2_staticBody;
	}
	else
	{
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		newBodyDef.type = b2_dynamicBody;
	}

	newBodyDef.position.Set(entity->getTransform()->position.x, entity->getTransform()->position.y);
	b2Body* newBody = world_.CreateBody(&newBodyDef);
	b2Fixture* newFixture = newBody->CreateFixture(&fixtureDef);
	entity->setRigidBody(std::shared_ptr<CRigidBody>(new CRigidBody(is_static, newBody, newFixture)));
	entity_to_bodies_[entity] = newBody;
}

void GatorPhysics::destroyBody(Entity* entity)
{
	world_.DestroyBody(entity_to_bodies_[entity]);
	entity_to_bodies_.erase(entity);
	entity->cRigidBody.reset();
}