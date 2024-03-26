#include "GatorPhysics.h"
#include "GameEngine.h"


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
	float worldY = GameEngine::GetInstance().window().getSize().y;
	for (auto node: entity_to_bodies_) {
		//If anything happened to the positions/rotation of the entities, update their physics bodies
		//float physicsY = (worldY - node.first->getComponent<CTransform>()->position.y);
		node.second->SetTransform(b2Vec2(node.first->getComponent<CTransform>()->position.x, node.first->getComponent<CTransform>()->position.y), node.first->getComponent<CTransform>()->angle);
		node.second->SetAwake(true);
	}

	//Step the physics world
	world_.Step(timeStep, velocityIterations, positionIterations);

	for (auto node : entity_to_bodies_) {
		//Update the positions/rotation of the entities to match the physics bodies
		//float entityY = (worldY - node.second->GetPosition().y);
		node.first->getComponent<CTransform>()->position = Vec2(node.second->GetPosition().x, node.second->GetPosition().y);
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
	bool has_sprite = entity->hasComponent<CSprite>();
	bool has_animations = entity->hasComponent<CAnimation>();
	sf::Sprite& sprite = has_sprite ? entity->getComponent<CSprite>()->sprite_ : entity->getComponent<CAnimation>()->animation_.sprite_;
	newBox.SetAsBox(sprite.getLocalBounds().getSize().x/2, sprite.getLocalBounds().getSize().y/2);

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
	float worldY = GameEngine::GetInstance().window().getSize().y;

	newBodyDef.position.Set(entity->getComponent<CTransform>()->position.x, worldY - entity->getComponent<CTransform>()->position.y - 150);
	b2Body* newBody = world_.CreateBody(&newBodyDef);
	b2Fixture* newFixture = newBody->CreateFixture(&fixtureDef);
	entity->addComponent<CRigidBody>(is_static, newBody, newFixture);
	entity_to_bodies_[entity] = newBody;
}

void GatorPhysics::destroyBody(Entity* entity)
{
	world_.DestroyBody(entity_to_bodies_[entity]);
	entity_to_bodies_.erase(entity);
	entity->getComponent<CRigidBody>().reset();
}