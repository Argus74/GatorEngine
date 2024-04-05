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

float GatorPhysics::getScale()
{
	return scale_;
}

std::map<Entity*, b2Body*>& GatorPhysics::GetEntityToBodies()
{
	return entity_to_bodies_;
}

void GatorPhysics::update()
{

	float worldY = GameEngine::GetInstance().window().getSize().y;

	
	for (auto node : entity_to_bodies_)
	{
		//Update the physics bodies to match the static/dynamic status of the entities
		Entity* entity = node.first;
		b2Body* body = entity->getComponent<CRigidBody>()->body;
		//If it is not static, then we need to update the physics body
		if (!entity->getComponent<CRigidBody>()->static_body)
		{
			body->SetType(b2_dynamicBody);
		}
		else {
			body->SetType(b2_staticBody);
		}

		//If anything happened to the positions/rotation/scale of the entities, update their physics bodies
		//float physicsY = (worldY - node.first->getComponent<CTransform>()->position.y);
		float physicsY = (worldY - node.first->getComponent<CTransform>()->position.y) / scale_;
		float physicsX = node.first->getComponent<CTransform>()->position.x / scale_;
		node.second->SetTransform(b2Vec2(physicsX, physicsY), node.first->getComponent<CTransform>()->angle);
		b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(node.second->GetFixtureList()[0].GetShape());

		//Update the size of the physics body to match the size of the entity
		float xScale = entity->getComponent<CTransform>()->scale.x;
		float yScale = entity->getComponent<CTransform>()->scale.y;
		float newWidth;
		float newHeight;
		if (entity->hasComponent<CAnimation>())
		{
			newWidth = entity->getComponent<CAnimation>()->animation.sprite.getLocalBounds().getSize().x / 2 * xScale / scale_;
			newHeight = entity->getComponent<CAnimation>()->animation.sprite.getLocalBounds().getSize().y / 2 * yScale / scale_;
		}
		else if (entity->hasComponent<CSprite>())
		{
			newWidth = entity->getComponent<CSprite>()->sprite.getLocalBounds().getSize().x / 2 * xScale / scale_;
			newHeight = entity->getComponent<CSprite>()->sprite.getLocalBounds().getSize().y / 2 * yScale / scale_;
		}
		shape->SetAsBox(newWidth, newHeight);
		node.second->SetAwake(true);
	}
	
	//Step the physics world
	world_.Step(time_step_, velocity_iterations_, position_iterations_);

	for (auto node : entity_to_bodies_) {
		//Update the positions/rotation of the entities to match the physics bodies
		//float entityY = (worldY - node.second->GetPosition().y);
		float entityY = ((node.second->GetPosition().y * scale_) - worldY) * -1 ;
		float entityX = node.second->GetPosition().x * scale_;
		node.first->getComponent<CTransform>()->position = Vec2(entityX, entityY);
		//node.first->getComponent<CTransform>()->angle = node.second->GetAngle();
	}
}

void GatorPhysics::setGravity(Vec2 gravity)
{
	gravity_ = b2Vec2(gravity.x, gravity.y);
}

void GatorPhysics::createBody(Entity* entity, bool isStatic)
{
	b2BodyDef newBodyDef;
	b2PolygonShape newBox;
	bool has_sprite = entity->hasComponent<CSprite>();
	bool has_animations = entity->hasComponent<CAnimation>();
	sf::Sprite& sprite = has_sprite ? entity->getComponent<CSprite>()->sprite : entity->getComponent<CAnimation>()->animation.sprite;
	float boxWidth = sprite.getLocalBounds().getSize().x * (1/scale_) / 2;
	float boxHeight = sprite.getLocalBounds().getSize().y * (1/scale_) / 2;
	newBox.SetAsBox(boxWidth, boxHeight);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &newBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	if (isStatic)
	{
		newBodyDef.type = b2_staticBody;
	}
	else
	{
		newBodyDef.type = b2_dynamicBody;
	}
	float worldY = GameEngine::GetInstance().window().getSize().y;

	float boxX = entity->getComponent<CTransform>()->position.x / scale_;
	float boxY = (worldY - entity->getComponent<CTransform>()->position.y) / scale_;
	float offset = 150 / scale_;
	newBodyDef.position.Set(boxX,boxY - offset);
	b2Body* newBody = world_.CreateBody(&newBodyDef);
	newBody->SetFixedRotation(true);
	b2Fixture* newFixture = newBody->CreateFixture(&fixtureDef);
	entity->addComponent<CRigidBody>(isStatic, newBody, newFixture);
	entity_to_bodies_[entity] = newBody;
}

void GatorPhysics::destroyBody(Entity* entity)
{
	world_.DestroyBody(entity_to_bodies_[entity]);
	entity_to_bodies_.erase(entity);
	entity->getComponent<CRigidBody>().reset();
}