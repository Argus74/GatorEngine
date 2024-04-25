#include "GatorPhysics.h"

#include "GameEngine.h"

GatorPhysics::GatorPhysics() {

    world_.SetContactListener(this);
}

GatorPhysics& GatorPhysics::GetInstance() {
    static GatorPhysics instance_;
    return instance_;
}

b2World* GatorPhysics::getWorld() {
    return nullptr;
}

float GatorPhysics::getScale() {
    return scale_;
}

std::map<Entity*, b2Body*>& GatorPhysics::GetEntityToBodies() {
    return entity_to_bodies_;
}

void GatorPhysics::update() {

    float worldY = GameEngine::GetInstance().window().getSize().y;

    for (auto node : entity_to_bodies_) {
        Entity* entity = node.first;
        if (entity == nullptr)
            continue;
        if (!entity->initialized || !entity->hasComponent<CRigidBody>())
            continue;

        b2Body* body = entity->getComponent<CRigidBody>()->body;

        //First check if the node is disabled, if it is, then we don't need to update the physics body

        if (entity->isDisabled()) {
            body->SetEnabled(false);
            continue;
        } else {
            body->SetEnabled(true);
        }
        //Update the physics bodies to match the static/dynamic status of the entities
        //If it is not static, then we need to update the physics body
        if (!entity->getComponent<CRigidBody>()->static_body) {
            body->SetType(b2_dynamicBody);
        } else {
            body->SetType(b2_staticBody);
        }

        //If anything happened to the positions/rotation/scale of the entities, update their physics bodies
        //and trigger boxes
        //float physicsY = (worldY - node.first->getComponent<CTransform>()->position.y);
        float physicsY = (worldY - node.first->getComponent<CTransform>()->position.y) / scale_;
        float physicsX = node.first->getComponent<CTransform>()->position.x / scale_;
        node.second->SetTransform(b2Vec2(physicsX, physicsY),
                                  node.first->getComponent<CTransform>()->angle);

        b2Fixture* firstFixture = node.second->GetFixtureList();
        b2Fixture* secondFixture = firstFixture->GetNext();

        b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(firstFixture->GetShape());
        b2PolygonShape* sensorShape = dynamic_cast<b2PolygonShape*>(secondFixture->GetShape());
        //Update the size of the physics body to match the size of the entity
        float xScale = entity->getComponent<CTransform>()->scale.x;
        float yScale = entity->getComponent<CTransform>()->scale.y;
        float newWidth;
        float newHeight;
        if (entity->hasComponent<CAnimation>()) {
            newWidth =
                entity->getComponent<CAnimation>()->animation.sprite.getLocalBounds().getSize().x /
                2 * xScale / scale_;
            newHeight =
                entity->getComponent<CAnimation>()->animation.sprite.getLocalBounds().getSize().y /
                2 * yScale / scale_;
        } else if (entity->hasComponent<CSprite>()) {
            newWidth = entity->getComponent<CSprite>()->sprite.getLocalBounds().getSize().x / 2 *
                       xScale / scale_;
            newHeight = entity->getComponent<CSprite>()->sprite.getLocalBounds().getSize().y / 2 *
                        yScale / scale_;
        }
        shape->SetAsBox(newWidth, newHeight);
        sensorShape->SetAsBox(newWidth, newHeight);
        //Update the densities and friction of the physics bodies
        b2Fixture* fixture = body->GetFixtureList();
        fixture->SetDensity(entity->getComponent<CRigidBody>()->density);
        fixture->SetFriction(entity->getComponent<CRigidBody>()->friction);

        //Update the densities and friction of the sensor fixtures
        fixture = body->GetFixtureList()->GetNext();
        fixture[1].SetDensity(entity->getComponent<CRigidBody>()->density);
        fixture->SetFriction(entity->getComponent<CRigidBody>()->friction);

        body->ResetMassData();
        node.second->SetAwake(true);
    }

    //Step the physics world
    world_.Step(time_step_, velocity_iterations_, position_iterations_);
    //we will now print out the centroids of the bodies as well as the positions of the physics bodies.
    //We will also print out the widths
    //and heights of the physics bodies.

    for (auto node : entity_to_bodies_) {
        b2Vec2 position = node.second->GetPosition();
        b2PolygonShape* shape =
            dynamic_cast<b2PolygonShape*>(node.second->GetFixtureList()[0].GetShape());
        float width = shape->m_vertices[2].x - shape->m_vertices[0].x;
        float height = shape->m_vertices[2].y - shape->m_vertices[0].y;
    }

    for (auto node : entity_to_bodies_) {
        //Update the positions/rotation of the entities to match the physics bodies
        //float entityY = (worldY - node.second->GetPosition().y);
        float entityY = ((node.second->GetPosition().y * scale_) - worldY) * -1;
        float entityX = node.second->GetPosition().x * scale_;
        node.first->getComponent<CTransform>()->position = Vec2(entityX, entityY);
        //node.first->getComponent<CTransform>()->angle = node.second->GetAngle();
    }

    //Check for collisions and send events and update the character components of the entities
    /*for (auto node : entity_to_bodies_) {
		Entity* entity = node.first;
		b2Body* body = node.second;
		b2ContactEdge* contactEdge = body->GetContactList();
		while (contactEdge != nullptr)
		{

			contactEdge = contactEdge->next;
		}
	}*/
}

void GatorPhysics::setGravity(Vec2 gravity) {
    gravity_ = b2Vec2(gravity.x, gravity.y);
}

void GatorPhysics::createBody(Entity* entity, bool isStatic) {
    b2BodyDef newBodyDef;
    b2PolygonShape newBox;
    bool hasSprite = entity->hasComponent<CSprite>();
    bool hasAnimations = entity->hasComponent<CAnimation>();

    if (!hasSprite && !hasAnimations) {
        std::cout << "Entity does not have a sprite or animation" << std::endl;
        return;
    }

    sf::Sprite& sprite = hasSprite ? entity->getComponent<CSprite>()->sprite
                                   : entity->getComponent<CAnimation>()->animation.sprite;
    float boxWidth = sprite.getLocalBounds().getSize().x * (1 / scale_) / 2;
    float boxHeight = sprite.getLocalBounds().getSize().y * (1 / scale_) / 2;
    newBox.SetAsBox(boxWidth, boxHeight);
    newBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

    //Create a fixtuure to represent the physical body that will collide with other bodies
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &newBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    if (isStatic) {
        newBodyDef.type = b2_staticBody;
    } else {
        newBodyDef.type = b2_dynamicBody;
    }

    //Create a fixture to represent the collider that will sense collisions
    b2FixtureDef sensorFixtureDef;
    sensorFixtureDef.shape = &newBox;
    sensorFixtureDef.isSensor = true;

    float worldY = GameEngine::GetInstance().window().getSize().y;

    float boxX = entity->getComponent<CTransform>()->position.x / scale_;
    float boxY = (worldY - entity->getComponent<CTransform>()->position.y) / scale_;
    float offset = 150 / scale_;
    newBodyDef.position.Set(boxX, boxY - offset);
    b2Body* newBody = world_.CreateBody(&newBodyDef);
    newBody->SetFixedRotation(true);
    b2Fixture* newFixture = newBody->CreateFixture(&fixtureDef);
    b2Fixture* sensorFixture = newBody->CreateFixture(&sensorFixtureDef);
    entity->addComponent<CRigidBody>(isStatic, newBody, newFixture, sensorFixture);

    //Add the entity to the user data of the body so we can access the entity from the body
    entity_to_bodies_[entity] = newBody;
}

void GatorPhysics::destroyBody(Entity* entity) {
    world_.DestroyBody(entity_to_bodies_[entity]);
    entity_to_bodies_.erase(entity);
    entity->getComponent<CRigidBody>().reset();
}

void GatorPhysics::BeginContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Fixture* sensorFixture = fixtureA->IsSensor() ? fixtureA : fixtureB;
    Entity* entity = reinterpret_cast<Entity*>(sensorFixture->GetBody()->GetUserData().pointer);
    if (sensorFixture) {
        //Check if the sensor is below the entity
        b2Vec2 normal = contact->GetManifold()->localNormal;
        if (normal.y < 0.5) {
            //std::cout << entity->getComponent<CName>()->kComponentName << "Grounded" << std::endl;
            entity->getComponent<CRigidBody>()->is_grounded = true;
        } else {
            entity->getComponent<CRigidBody>()->is_grounded = false;
        }
    }
    b2Fixture* otherFixture = sensorFixture == fixtureA ? fixtureB : fixtureA;
    Entity* otherEntity =  reinterpret_cast<Entity*>(
        otherFixture->GetBody()->GetUserData().pointer);

    //For each node in the lua_states map, we will call the OnTouched function if the raw pointer in 
//the map is equal to the enitity that was touched
    for (auto node : GameEngine::GetInstance().lua_states) {
    	if (node.first.get() == entity) {
			sol::state& sol_state = node.second->GetSolState();
			sol_state["OnTouched"](otherEntity);
		}
    }   
}

void GatorPhysics::EndContact(b2Contact* contact) {}

void GatorPhysics::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}

void GatorPhysics::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

void GatorPhysics::clearBodies() {
    for (auto entity : entity_to_bodies_) {
        world_.DestroyBody(entity.second);
    }
    entity_to_bodies_.clear();
}

void GatorPhysics::ResetWorld() {
    // First, clear all existing bodies from the world
    for (auto entity : entity_to_bodies_) {
        world_.DestroyBody(entity.second);
    }

    // Optionally re-add bodies if entities are supposed to persist across resets
    for (auto& pair : entity_to_bodies_) {
        Entity* entity = pair.first;
        // Ensure the entity is correctly setup to recreate its body
        if (entity->hasComponent<CRigidBody>()) {
            bool isStatic = entity->getComponent<CRigidBody>()->static_body;
            createBody(entity, isStatic);  // Recreate the body for the entity
        }
    }
}