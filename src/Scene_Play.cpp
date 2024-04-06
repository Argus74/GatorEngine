#include "Scene_Play.h"
#include "GatorPhysics.h"
#include "AssetManager.h"
#include "GameEngine.h"
#include "components/Component.h"

#include <fstream>
#include <iostream>

Scene_Play::Scene_Play()
{
	spawnPlayer();
	// Create a platform and a tree:
	std::shared_ptr<Entity> ground = m_entityManager.addEntity("Ground");
	// Create a platform and a tree:
	std::shared_ptr<Entity> ground2 = m_entityManager.addEntity("Ground");
	// Create a platform and a tree:
	std::shared_ptr<Entity> ground3 = m_entityManager.addEntity("Ground");
	// The parameters to construct a transform are position and scale and angle of rotation
	ground->addComponent<CTransform>(Vec2(224, 300), Vec2(1, 1), 0);
	ground->addComponent<CSprite>("Ground");
	ground->addComponent<CName>("Ground");
	ground->addComponent<CInformation>();
	//ground->getComponent<CSprite>()->texture_ = GameEngine::GetInstance().assets().GetTexture("Ground");
	//Need to select ground portion of the texture
	ground->getComponent<CSprite>()->setTexturePortion(sf::IntRect(95, 0, 48, 48));
	GatorPhysics::GetInstance().createBody(ground.get(), true);


	// The parameters to construct a transform are position and scale and angle of rotation
	ground2->addComponent<CTransform>(Vec2(272, 300), Vec2(1, 1), 0);
	ground2->addComponent<CSprite>("Ground");
	ground2->addComponent<CName>("Ground2");
	ground2->addComponent<CInformation>();
	//ground->getComponent<CSprite>()->texture_ = GameEngine::GetInstance().assets().GetTexture("Ground");
	//Need to select ground portion of the texture
	ground2->getComponent<CSprite>()->setTexturePortion(sf::IntRect(95, 0, 48, 48));
	GatorPhysics::GetInstance().createBody(ground2.get(), true);


	// The parameters to construct a transform are position and scale and angle of rotation
	ground3->addComponent<CTransform>(Vec2(320, 300), Vec2(1, 1), 0);
	ground3->addComponent<CSprite>("Ground");
	ground3->addComponent<CName>("Ground3");
	ground3->addComponent<CInformation>();
	//ground->getComponent<CSprite>()->texture_ = GameEngine::GetInstance().assets().GetTexture("Ground");
	//Need to select ground portion of the texture
	ground3->getComponent<CSprite>()->setTexturePortion(sf::IntRect(95, 0, 48, 48));
	GatorPhysics::GetInstance().createBody(ground3.get(), true);
	/*std::shared_ptr<Entity> tree = EntityManager::addEntity("Tree");
	tree->addComponent<CTransform>(Vec2(200, 400), Vec2(20, 50));
	tree->addComponent<CSprite>(m_game->assets().getTexture("Tree"));*/



}

Scene_Play::Scene_Play(const std::string &levelPath) : m_levelPath(levelPath)
{
	LoadScene(levelPath);
}

void Scene_Play::LoadScene(const std::string &filename)
{
	// reset the entity manager every time we load a level
	EntityManager::GetInstance().reset();

	// TODO: Read the level file and spawn entities based on the data

	spawnPlayer();

	// some sample entities
	auto brick = m_entityManager.addEntity("tile");
	brick->addComponent<CSprite>();
	brick->getComponent<CSprite>()->texture_ = GameEngine::GetInstance().assets().GetTexture("DefaultSprite");
	brick->addComponent<CTransform>(Vec2(96, 200), Vec2(5, 5), 0);
}

void Scene_Play::spawnPlayer()
{
	// here is a sample player entity which you can use to construct other entities
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>();
	m_player->getComponent<CAnimation>()->animation_ = GameEngine::GetInstance().assets().GetAnimation("DefaultAnimation");
	m_player->addComponent<CTransform>(Vec2(224, 200));
	m_player->addComponent<CUserInput>();
	m_player->addComponent<CName>("Player1");
	m_player->addComponent<CInformation>();
	GatorPhysics::GetInstance().createBody(m_player.get(), false);

	// TODO: be sure to add the remaining components to the player
}

void Scene_Play::update()
{
	m_entityManager.update();
	sUserInput();
	sMovement();
	sPhysics();
	sCollision();
	sBackground();
	sRender();
	//sRenderColliders();
	//GatorPhysics &physics = GatorPhysics::GetInstance();
}

void Scene_Play::sCollision()
{
	GatorPhysics::GetInstance().update();
}

void Scene_Play::sUserInput()
{
	// Clear the bus from previous frame's input
	ActionBus::GetInstance().Clear();
	//std::cout << "sUserInput" << std::endl;
	sf::Event event;
	while (GameEngine::GetInstance().window().pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			GameEngine::GetInstance().window().close();
		}

		// Update the SFML's render window dimensions to prevent weird sprite scaling
		if (event.type == sf::Event::Resized)
		{
			sf::FloatRect view(0, 0, event.size.width, event.size.height);
			GameEngine::GetInstance().window().setView(sf::View(view));
		}

		// Lambda to process key or mouse events for the player
		auto processInputEvent = [](const sf::Event &event, const sf::Event::EventType &eventType)
		{
			auto &entities = EntityManager::GetInstance().getEntities();
			for (auto &entity : entities)
			{
				// Skip entities without a cUserInput component
				if (!entity->getComponent<CUserInput>())
					continue;

				auto findAndDispatch = [entity](auto &inputMap, const auto &eventButton)
				{
					// For each key in the inputmap
					for (auto &actionKeys : inputMap)
					{
						if (actionKeys.first == eventButton)
						{
							ActionBus::GetInstance().Dispatch(entity, actionKeys.second);
						}
					}
				};

				// Find potential actions and dispatch them based on the event type
				if (eventType == sf::Event::KeyPressed)
				{
					auto &inputMap = entity->getComponent<CUserInput>()->keyMap;
					findAndDispatch(inputMap, event.key.code);
				}
				else
				{
					auto &inputMap = entity->getComponent<CUserInput>()->mouseMap;
					findAndDispatch(inputMap, event.mouseButton.button);
				}
			}
		};

		// Process key or mouse input events with the lambda
		if (event.type == sf::Event::KeyPressed)
		{
			processInputEvent(event, sf::Event::KeyPressed);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			processInputEvent(event, sf::Event::MouseButtonPressed);
		}
	}

	// sRender outside of testing check here?
}

void Scene_Play::sPhysics()
{

	//First check if any new entities have a new rigid body component and
	// have not been added to the physics world
	
	for (auto entity : EntityManager::GetInstance().getEntities())
	{
		if (entity->hasComponent<CRigidBody>())
		{
			auto rigidBodyComponent = entity->getComponent<CRigidBody>();
			std::map<Entity*, b2Body*>& entity_to_bodies_ = GatorPhysics::GetInstance().GetEntityToBodies();
			//If the entity is not in the physics world, add it
			if (entity_to_bodies_.find(entity.get()) == entity_to_bodies_.end())
			{
				GatorPhysics::GetInstance().createBody(entity.get(), rigidBodyComponent->staticBody);
			}
		}
	}

	// For each entity move them based on their velocity and physics components
	for (auto entity : EntityManager::GetInstance().getEntities())
	{
		if (entity->hasComponent<CTransform>())
		{
			auto transform = entity->getComponent<CTransform>();
			// Update the position based on the velocity
			transform->position = transform->position + transform->velocity;
		}
	}

	// Any other movement that should be done based on other physics components
	// should be done below here
}

void Scene_Play::onEnd()
{
	// TODO: When the scene ends, change back to the MENU scene
	//		 use m_game->changeScene(correct params);
}

void Scene_Play::sRender()
{
	auto &entityManager = EntityManager::GetInstance();

	std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntitiesRenderingList();

	for (auto &entity : entityList)
	{ // Looping through entity list and drawing the sprites to the render window.
		if (entity->hasComponent<CSprite>())
		{
			auto transformComponent = entity->getComponent<CTransform>();
			Vec2 scale = transformComponent->scale;
			Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
			auto spriteComponent = entity->getComponent<CSprite>();
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;

			// Set the origin of the sprite to its center
			sf::FloatRect bounds = spriteComponent->sprite_.getLocalBounds();
			spriteComponent->sprite_.setOrigin(bounds.width / 2, bounds.height / 2);
			spriteComponent->sprite_.setPosition(position.x, position.y + yOffset);
			spriteComponent->sprite_.setScale(scale.x, scale.y);
      
			//Rotation
			float angle = transformComponent->angle * -1;
			spriteComponent->sprite_.setRotation(angle);
			

			if (spriteComponent->drawSprite_)
				GameEngine::GetInstance().window().draw(spriteComponent->sprite_);
		}

		if (entity->hasComponent<CAnimation>())
		{
			auto transformComponent = entity->getComponent<CTransform>();
			Vec2 scale = transformComponent->scale;
			Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
			auto animationComponent = entity->getComponent<CAnimation>();
			animationComponent->changeSpeed();
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;
			sf::Sprite sprite(animationComponent->animation_.sprite_);
			// Set the origin of the sprite to its center
			sf::FloatRect bounds = sprite.getLocalBounds();
			sprite.setOrigin(bounds.width / 2, bounds.height / 2);

			// Set the position of the sprite to the center position
			sprite.setPosition(position.x, position.y + yOffset);
			sprite.setScale(scale.x, scale.y);
			float angle = transformComponent->angle * -1;
			sprite.setRotation(angle);
			GameEngine::GetInstance().window().draw(sprite);
			animationComponent->update();
		}
	}
}

void Scene_Play::sRenderColliders() {
	auto& entityManager = EntityManager::GetInstance();

	std::vector<std::shared_ptr<Entity>>& entityList = entityManager.getEntities();

	for (auto& entity : entityList)
	{ // Looping through entity list and drawing the sprites to the render window.
		if (entity->hasComponent<CRigidBody>())
		{
			auto rigidBodyComponent = entity->getComponent<CRigidBody>();
			b2Vec2 position = rigidBodyComponent->body->GetPosition();
			//These sizes are half widths and half heights
			b2Vec2 size = rigidBodyComponent->body->GetFixtureList()->GetAABB(0).GetExtents();
			float xScale = entity->getComponent<CTransform>()->scale.x;
			float yScale = entity->getComponent<CTransform>()->scale.y;
			float entityWidth = size.x * 2 / xScale / GatorPhysics::GetInstance().getScale();
			float entityHeight = size.y * 2 / yScale / GatorPhysics::GetInstance().getScale();
			auto spriteComponent = sf::RectangleShape();
			spriteComponent.setOrigin(entityWidth / 2, entityHeight / 2);
			spriteComponent.setFillColor(sf::Color::White);
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;
			float worldY = GameEngine::GetInstance().window().getSize().y;
			float entityY = ((position.y / GatorPhysics::GetInstance().getScale()) - worldY) * -1;
			float entityX = position.x / GatorPhysics::GetInstance().getScale();
			spriteComponent.setPosition(entityX, entityY + yOffset); // Removed the +150 from the y position
			
			spriteComponent.setSize(sf::Vector2f(entityWidth, entityHeight));
			GameEngine::GetInstance().window().draw(spriteComponent);
		}
	}
}


void Scene_Play::sMovement()
{
	for (auto entity : EntityManager::GetInstance().getEntities()) {
		if (!entity->hasComponent<CTransform>() || !entity->hasComponent<CRigidBody>()) continue;
		float speed = 5.0;
		//Vec2 finalVelocity = entity->getComponent<CTransform>()->velocity;
		//Vec2 finalAcceleration = Vec2(0, 0);
		b2Body* body = GatorPhysics::GetInstance().GetEntityToBodies()[entity.get()];
		
		b2Vec2 resultMovement = b2Vec2(0, 0);
		if (ActionBus::GetInstance().Received(entity, MoveRight))
			resultMovement += b2Vec2(speed, 0);
		//finalVelocity = finalVelocity + Vec2(speed, 0);

		if (ActionBus::GetInstance().Received(entity, MoveLeft))
			resultMovement += b2Vec2(-speed, 0);
		//finalVelocity = finalVelocity + Vec2(-speed, 0);

		if (ActionBus::GetInstance().Received(entity, Jump))
			body->ApplyForceToCenter(b2Vec2(0, 30), true);

		b2Vec2 velocity = body->GetLinearVelocity();
		resultMovement = b2Vec2(resultMovement.x, velocity.y);

		body->SetLinearVelocity(resultMovement);


	}
}

// Handles clearing of window and drawing scene background
void Scene_Play::sBackground() {
	// Find first component of type CBackground and draw it
	auto entityList = EntityManager::GetInstance().getEntities();
	for (auto& entity : entityList) {
		if (entity->hasComponent<CBackgroundColor>()) {
			auto background = entity->getComponent<CBackgroundColor>();
			GameEngine::GetInstance().window().clear(background->color);
			return;
		}
	}

	// Otherwise, default to a black background
	GameEngine::GetInstance().window().clear(sf::Color(0, 0, 0));
}

