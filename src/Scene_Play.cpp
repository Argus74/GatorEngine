#include "Scene_Play.h"
#include "GatorPhysics.h"
#include "AssetManager.h"
#include "GameEngine.h"
#include "components/Component.h"

#include <fstream>
#include <iostream>

Scene_Play::Scene_Play()
{
	//spawnPlayer();
	// Create a platform and a tree:
	std::shared_ptr<Entity> ground = m_entityManager.addEntity("Ground");
	// The parameters to construct a transform are position and scale and angle of rotation
	ground->addComponent<CTransform>(Vec2(224, 300), Vec2(1, 1), 0);
	ground->addComponent<CSprite>("Ground");
	ground->addComponent<CName>("Ground");
	//ground->getComponent<CSprite>()->texture_ = GameEngine::GetInstance().assets().GetTexture("Ground");
	//Need to select ground portion of the texture
	ground->getComponent<CSprite>()->setTexturePortion(sf::IntRect(95, 0, 48, 48));
	GatorPhysics::GetInstance().createBody(ground.get(), true);

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
	sAnimation();
	sRender();
	GatorPhysics &physics = GatorPhysics::GetInstance();
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
				std::cout << "Event button: before " << std::endl;
				// Skip entities without a cUserInput component
				if (!entity->getComponent<CUserInput>())
					continue;

				auto findAndDispatch = [entity](auto &inputMap, const auto &eventButton)
				{
					// For each key in the inputmap
					std::cout << "Event button: " << eventButton << std::endl;
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

	if (Editor::state == Editor::State::Testing)
	{
		EntityManager::GetInstance().update();
		// other systems here
		m_currentFrame++;
	}
	else
	{
		m_currentFrame = 0;
		/*
			m_player->cTransform->position = Vec2(400, 400);
			- resetting player position back to a mock position if not testing, could be an idea to reset everything here?
		*/
	}

	// sRender outside of testing check here?
}

void Scene_Play::sPhysics()
{
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

void Scene_Play::sAnimation()
{
	// Need to add GetComponent, AddComponent templates to entity.
	auto &entityManager = EntityManager::GetInstance();
	std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntities();

	for (auto &entity : entityList)
	{
		if (entity->hasComponent<CAnimation>())
		{
			auto transformComponent = entity->getComponent<CTransform>();
			Vec2 scale = transformComponent->scale;
			Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
			auto animationComponent = entity->getComponent<CAnimation>();
			animationComponent->changeSpeed();
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;
			sf::Sprite sprite(animationComponent->animation_.sprite_);
			sprite.setPosition(position.x, position.y + yOffset); //Removed the +150 from the y position
			sprite.setScale(scale.x, scale.y);

			GameEngine::GetInstance().window().draw(sprite);
			animationComponent->update();
		}
	}
}

void Scene_Play::sRender()
{
	auto &entityManager = EntityManager::GetInstance();

	std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntities();

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

			// Set the position of the sprite to the center position
			spriteComponent->sprite_.setPosition(position.x, position.y + yOffset);
			spriteComponent->sprite_.setScale(scale.x, scale.y);

			if (spriteComponent->drawSprite_)
				GameEngine::GetInstance().window().draw(spriteComponent->sprite_);
		}
	}
}

void Scene_Play::sMovement()
{
	for (auto entity : EntityManager::GetInstance().getEntities()) {
		if (!entity->hasComponent<CTransform>()) continue;
		float speed = 5.0;
		if (ActionBus::GetInstance().Received(entity, MoveRight))
			entity->getComponent<CTransform>()->velocity = Vec2(speed, 0);
		else if (ActionBus::GetInstance().Received(entity, MoveLeft))
			entity->getComponent<CTransform>()->velocity = Vec2(-speed, 0);		
		else
			entity->getComponent<CTransform>()->velocity = Vec2(0, 0);
	}
}

// void Scene_Play::sAnimation()
//{
//	if (!m_paused) { GameEngine::GetInstance().window().clear(sf::Color(100, 100, 255)); }
//	else { GameEngine::GetInstance().window().clear(sf::Color(50, 50, 150)); }
//
//	// set the viewport of the window to be centered on the player if it's far
//	auto& pPos = m_player->getComponent<CTransform>()->position;
//	float windowCenterX = fmax(GameEngine::GetInstance().window().getSize().x / 2.0f, pPos.x);
//	sf::View view = GameEngine::GetInstance().window().getView();
//	// getCenter is incomplete
//	view.setCenter(windowCenterX, GameEngine::GetInstance().window().getSize().y - view.getCenter().y);
//	GameEngine::GetInstance().window().setView(view);
//
//	sf::CircleShape dot;
//	dot.setFillColor(sf::Color::Red);
//	dot.setRadius(8);
//	dot.setOrigin(8, 8);
//	dot.setPosition(m_mousePos.x, m_mousePos.y);
//	GameEngine::GetInstance().window().draw(dot);
//
//	// draw all Entity textures / animation
//
//	for (auto e : m_entityManager.getEntities())
//	{
//		auto& transform = e->getComponent<CTransform>();
//
//		if (e->hasComponent<CAnimation>())
//		{
//			auto& animation = e->getComponent<CAnimation>()->animation_;
//			animation.getSprite().setRotation(transform->angle);
//			animation.getSprite().setPosition(transform->position.x, transform->position.y);
//		}
//	}
//
// }