#include "GameEngine.h"


GameEngine::GameEngine() {}
GameEngine& GameEngine::GetInstance()
{
	// TODO: insert return statement here
	static GameEngine instance_;

	if (!instance_.initialized_)
	{
		instance_.init();
		instance_.initialized_ = true;
	}
	return instance_;
}

// void GameEngine::ChangeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endcurrent_scene_)
// {
// 	if (endcurrent_scene_)
// 	{
// 		current_scene_->onEnd();
// 	}

// 	// current_scene_ = sceneName;
// 	current_scene_ = scene;
// }

void GameEngine::quit()
{
	running_ = false;
}

void GameEngine::run()
{
	running_ = true;
}

sf::RenderWindow& GameEngine::window()
{
	return window_;
}

AssetManager& GameEngine::assets()
{
	return AssetManager::GetInstance();
}

void GameEngine::init()
{
	//Intializing all png files as textures in Start Assets folder
	AssetManager::GetInstance().IntializeAssets("assets/StartAssets");
	Animation ani = Animation("DefaultAnimation", AssetManager::GetInstance().GetTexture("DefaultAnimation"), 11, 1);
	AssetManager::GetInstance().AddAnimation("DefaultAnimation", ani);
	Animation ani2 = Animation("RunningAnimation", AssetManager::GetInstance().GetTexture("RunningAnimation"), 12, 1);
	AssetManager::GetInstance().AddAnimation("RunningAnimation", ani2);

	if (!readFromJSONFile("last-scene.json")) {
		current_scene_path_ = "scenes/Default.scene";
	} 

	//123
	window_.setFramerateLimit(60);
}

void GameEngine::changeScene(const std::string& path) 
{
	EntityManager::GetInstance().reset();
	Editor::state = Editor::State::None;
	Editor::active_entity_ = nullptr;
	Scene scene;
	scene.readFromJSONFile(path);	
	current_scene_path_ = path;
	writeToJSONFile("last-scene.json");
}

void GameEngine::saveScene(const std::string& path) 
{
	Scene scene;
	scene.writeToJSONFile(path);
	current_scene_path_ = path;
	writeToJSONFile("last-scene.json");
}

void GameEngine::addEntitiesForTest() 
{
	// add player
	auto player = EntityManager::GetInstance().addEntity("player");
	player->addComponent<CAnimation>();
	player->getComponent<CAnimation>()->animation = AssetManager::GetInstance().GetAnimation("DefaultAnimation");
	player->addComponent<CTransform>(Vec2(224, 200));
	player->addComponent<CUserInput>();
	player->addComponent<CName>("Player1");
	GatorPhysics::GetInstance().createBody(player.get(), false);


	// add ground tile 
	auto ground = EntityManager::GetInstance().addEntity("Ground");
	// The parameters to construct a transform are position and scale and angle of rotation
	ground->addComponent<CTransform>(Vec2(224, 300), Vec2(1, 1), 0);
	ground->addComponent<CSprite>("Grass Tile");
	ground->addComponent<CName>("Ground");
	//ground->getComponent<CSprite>()->texture = GameEngine::GetInstance().assets().GetTexture("Ground");
	//Need to select ground portion of the texture
	GatorPhysics::GetInstance().createBody(ground.get(), true);
}

void GameEngine::update()
{
	EntityManager::GetInstance().update();
	sUserInput();
	addEntitiesForTest();

	if (Editor::state == Editor::State::Testing) {
		sTouchTrigger();
		sScripts();
		sMovement();
		sPhysics();
		sCollision();
	}

	sBackground();
	sRender();
	sUI();
	//sRenderColliders();
	//GatorPhysics &physics = GatorPhysics::GetInstance();


	// EntityManager::GetInstance().update();
	// sUserInput();

	// if (Editor::kState == Editor::State::Testing) {
	// 	sMovement();
	// 	sPhysics();
	// 	sCollision();
	// }

	// sAnimation();
	// sRender();
	// GatorPhysics &physics = GatorPhysics::GetInstance();
	// current_scene_->update();
}

void GameEngine::sUserInput()
{
	// Clear the bus from previous frame's input
	ActionBus::GetInstance().Clear();
	//std::cout << "sUserInput" << std::endl;
	sf::Event event;
	while (window_.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			window_.close();
		}

		// Update the SFML's render window dimensions to prevent weird sprite scaling
		if (event.type == sf::Event::Resized)
		{
			sf::FloatRect view(0, 0, event.size.width, event.size.height);
			window_.setView(sf::View(view));
		}

		// Editor-specific hotkeys
		if (Editor::active_entity_ && Editor::state != Editor::State::Testing) {
			// Ctrl+D to copy active entity
			if (event.type == sf::Event::KeyPressed && event.key.control && event.key.code == sf::Keyboard::D) {
				EntityManager::GetInstance().cloneEntity(Editor::active_entity_);
			}

			// Ctrl+X to delete active entity
			if (event.type == sf::Event::KeyPressed && event.key.control && event.key.code == sf::Keyboard::X) {
				EntityManager::GetInstance().removeEntity(Editor::active_entity_);
			}

			// Ctrl+Z hotkey does not exist. Good luck o7
		}

		// Lambda to process key or mouse events for the player
		auto processInputEvent = [](const sf::Event &event, const sf::Event::EventType &eventType)
		{
			auto &entities = EntityManager::GetInstance().getEntities();
			for (auto &entity : entities)
			{
				std::cout << "Event button: before " << std::endl;
				// Skip entities without a cUserInput component
				if (!entity->getComponent<CUserInput>() || entity->isDisabled())
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
					auto &inputMap = entity->getComponent<CUserInput>()->key_map;
					findAndDispatch(inputMap, event.key.code);
				}
				else
				{
					auto &inputMap = entity->getComponent<CUserInput>()->mouse_map;
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
		current_frame_++;
	}
	else
	{
		current_frame_ = 0;
		/*
			player_->cTransform->position = Vec2(400, 400);
			- resetting player position back to a mock position if not testing, could be an idea to reset everything here?
		*/
	}

	// sRender outside of testing check here?
}

void GameEngine::sTouchTrigger()
{
	auto& entities = EntityManager::GetInstance().getEntities();
	for (auto& entity : entities) {
		// Skip entities without a touch trigger component, or that are disabled
		if (!entity->hasComponent<CTouchTrigger>() || entity->isDisabled()) continue;
		auto touchTrigger = entity->getComponent<CTouchTrigger>();
		auto triggerRect = entity->GetRect();

		// If has touch trigger, check if it is touching any other entity
		for (auto& entityTouched : entities) {
			// Skip entities without the tag we're caring about
			if (touchTrigger->tag != entityTouched->getComponent<CInformation>()->tag) continue;

			// Check if the entity is touching the entity with the touch trigger
			auto entityTouchedRect = entityTouched->GetRect(5); // Add leeway to the entity touched rect
			if (triggerRect.intersects(entityTouchedRect)) {

				if (touchTrigger->action == UpdateCollectible)
				{  // Only proceeding with an action if their is an collectable component attached, and its nots a health
					if (entity->hasComponent<CCollectable>() && !entity->getComponent<CCollectable>()->is_health)
					{
						// We are going to be updating not the entity that is touched, but rather the Text correlated to the collectable
						auto collectableEnityText = EntityManager::GetInstance().getEntityByName(entity->getComponent<CCollectable>()->text_entity_name);
						if (collectableEnityText != nullptr && collectableEnityText->hasComponent<CText>())
							Interact(entity, collectableEnityText);
					}
				}
				else if (touchTrigger->action == UpdateHealth)
				{
					if (entity->hasComponent<CCollectable>() && entity->getComponent<CCollectable>()->is_health)
					{
						Interact(entity, entityTouched);
					}
				}
				else if (touchTrigger->action == GiveJump)
				{
					ActionBus::GetInstance().Dispatch(entityTouched, Jump);
				}
			}
		}
	}
}

void GameEngine::sScripts() 
{
	//First, check if there are any entities that have been given a script component. If so,
	//add them to map of entities to lua states
	for (auto entity : EntityManager::GetInstance().getEntities())
	{
		if (entity->hasComponent<CScript>() && !lua_states[entity])
		{
			//Verify that the script name that the user typed in the editor is valid
			std::string script_name = entity->getComponent<CScript>()->script_name;
			std::ifstream file(script_name);
			if (!file.good())
			{
				std::cout << "Invalid script name: " << script_name << std::endl;
				continue;
			}
			//Attach a script to the player instance
			
			std::shared_ptr<LuaState> new_lua_state = std::make_shared<LuaState>(entity->getComponent<CScript>()->script_name, entity);
			lua_states[entity] = new_lua_state;
			entity->getComponent<CScript>()->lua_state = new_lua_state.get();
		}
	}

	//For each lua state, update the script
	for (auto node : lua_states)
	{
		//For each lua state, update the script
		if (node.second)
		{
			node.second->Update();
		}
	}
}

void GameEngine::sMovement()
{
	for (auto entity : EntityManager::GetInstance().getEntities()) {
		if (!entity->hasComponent<CTransform>() || !entity->hasComponent<CRigidBody>() || entity->isDisabled()) continue;
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

void GameEngine::sPhysics()
{

	//First check if any new entities have a new rigid body component and
	// have not been added to the physics world
	
	for (auto entity : EntityManager::GetInstance().getEntities())
	{
		if (entity->hasComponent<CRigidBody>() && !entity->isDisabled())
		{
			auto rigidBodyComponent = entity->getComponent<CRigidBody>();
			std::map<Entity*, b2Body*>& entity_to_bodies_ = GatorPhysics::GetInstance().GetEntityToBodies();
			//If the entity is not in the physics world, add it
			if (entity_to_bodies_.find(entity.get()) == entity_to_bodies_.end())
			{
				GatorPhysics::GetInstance().createBody(entity.get(), rigidBodyComponent->static_body);
			}
		}
	}

	// For each entity move them based on their velocity and physics components
	for (auto entity : EntityManager::GetInstance().getEntities())
	{
		if (entity->hasComponent<CTransform>() && !entity->isDisabled())
		{
			auto transform = entity->getComponent<CTransform>();
			// Update the position based on the velocity
			transform->position = transform->position + transform->velocity;
		}
	}

	// Any other movement that should be done based on other physics components
	// should be done below here
}

void GameEngine::sCollision()
{
	GatorPhysics::GetInstance().update();
}

// void GameEngine::sAnimation()
// {
// 	// Need to add GetComponent, AddComponent templates to entity.
// 	auto &entityManager = EntityManager::GetInstance();
// 	std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntities();

// 	for (auto &entity : entityList)
// 	{
// 		if (entity->hasComponent<CAnimation>())
// 		{
// 			auto transformComponent = entity->getComponent<CTransform>();
// 			Vec2 scale = transformComponent->scale;
// 			Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
// 			auto animationComponent = entity->getComponent<CAnimation>();
// 			animationComponent->changeSpeed();
// 			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;
// 			sf::Sprite sprite(animationComponent->animation.sprite);
// 			sprite.setPosition(position.x, position.y + yOffset); //Removed the +150 from the y position
// 			sprite.setScale(scale.x, scale.y);
// 			window_.draw(sprite);

// 			if (Editor::kState == Editor::State::Testing) {
// 				animationComponent->update();
// 			}
// 		}
// 	}
// }

void GameEngine::sBackground() {
	// Find first component of type CBackground and draw it
	auto entityList = EntityManager::GetInstance().getEntities();
	for (auto& entity : entityList) {
		if (entity->hasComponent<CBackgroundColor>() && !entity->isDisabled()) {
			auto background = entity->getComponent<CBackgroundColor>();
			window_.clear(background->color);
			return;
		}
	}

	// Otherwise, default to a black background
	window_.clear(sf::Color(0, 0, 0));
}

void GameEngine::sRender()
{
	auto &entityManager = EntityManager::GetInstance();

	std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntitiesRenderingList();

	for (auto &entity : entityList)
	{ // Looping through entity list and drawing the sprites to the render window.
		if (entity->hasComponent<CSprite>() && !entity->isDisabled())
		{
			auto transformComponent = entity->getComponent<CTransform>();
			Vec2 scale = transformComponent->scale;
			Vec2 position = Editor::state == Editor::State::Testing ? transformComponent->position : transformComponent->origin; // getting the scale and positioning from the transform component in order to render sprite at proper spot
			auto spriteComponent = entity->getComponent<CSprite>();
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;

			// Set the origin of the sprite to its center
			sf::FloatRect bounds = spriteComponent->sprite.getLocalBounds();
			spriteComponent->sprite.setOrigin(bounds.width / 2, bounds.height / 2);
			spriteComponent->sprite.setPosition(position.x, position.y + yOffset);
			spriteComponent->sprite.setScale(scale.x, scale.y);
      
      //Rotation
			float angle = transformComponent->angle * -1;
			spriteComponent->sprite.setRotation(angle);
			

			if (spriteComponent->draw_sprite)
				window_.draw(spriteComponent->sprite);
		}

		if (entity->hasComponent<CAnimation>() && !entity->isDisabled())
		{
			auto transformComponent = entity->getComponent<CTransform>();
			Vec2 scale = transformComponent->scale;
			Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
			auto animationComponent = entity->getComponent<CAnimation>();
			animationComponent->changeSpeed();
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;
			sf::Sprite sprite(animationComponent->animation.sprite);
			// Set the origin of the sprite to its center
			sf::FloatRect bounds = sprite.getLocalBounds();
			sprite.setOrigin(bounds.width / 2, bounds.height / 2);

			// Set the position of the sprite to the center position
			sprite.setPosition(position.x, position.y + yOffset);
			sprite.setScale(scale.x, scale.y);
			float angle = transformComponent->angle * -1;
			sprite.setRotation(angle);
			window_.draw(sprite);
			if (Editor::state == Editor::State::Testing) {
				animationComponent->update();
			}
		}
	}
}

void GameEngine::sUI() {
	auto& entityManager = EntityManager::GetInstance();

	std::vector<std::shared_ptr<Entity>>& entityList = entityManager.getUIRenderingList(); // We only iterate through the UI rendering list 

	for (auto& entity : entityList) {

		if (entity->hasComponent<CHealth>() && entity->getComponent<CHealth>()->draw_health && !entity->isDisabled())
		{ // Health Bar 
			auto healthComponent = entity->getComponent<CHealth>();

			sf::Sprite backHealth(healthComponent->back_health_bar);
			sf::Sprite frontHealth(healthComponent->front_health_bar);

			//Making the sprite for the front Health bar
			healthComponent->Update();

			// Set the origin of the sprite to its center
			sf::FloatRect bounds = backHealth.getLocalBounds();
			backHealth.setOrigin(bounds.width / 2, bounds.height / 2);
			sf::FloatRect bounds2 = frontHealth.getLocalBounds();
			//Hard coded for now, the best way to allow the user to customize health bars would be a different approach
			frontHealth.setOrigin(bounds.width / 2 - 7, bounds2.height / 2);

			// Set the position of the sprite to the center position
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;

			Vec2 scale = healthComponent->health_bar_scale;

			if (healthComponent->follow_entity) {
				// The position is above
				Vec2 position = entity->getComponent<CTransform>()->position + healthComponent->health_bar_offset;
				backHealth.setPosition(position.x, position.y + yOffset);
				backHealth.setScale(scale.x, scale.y);

				frontHealth.setPosition(position.x, position.y + yOffset);
				frontHealth.setScale(scale.x, scale.y);
			}
			else {
				Vec2 position = healthComponent->health_bar_position;
				backHealth.setPosition(position.x, position.y + yOffset);
				backHealth.setScale(scale.x, scale.y);

				// Set the position of the sprite to the center position
				frontHealth.setPosition(position.x, position.y + yOffset);
				frontHealth.setScale(scale.x, scale.y);
			}

			GameEngine::GetInstance().window().draw(backHealth);
			GameEngine::GetInstance().window().draw(frontHealth);

		}

		if (entity->hasComponent<CText>() && !entity->isDisabled())
		{
			auto transformComponent = entity->getComponent<CTransform>(); // Transform related 
			Vec2 scale = transformComponent->scale;
			Vec2 position = transformComponent->position;
			float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;

			auto textComponent = entity->getComponent<CText>(); // Setting the properties of the text

			std::string outputString = textComponent->message;
			if (textComponent->is_counter) {
				outputString += std::to_string(textComponent->counter);
			}

			textComponent->text.setFont(textComponent->font);
			textComponent->text.setString(outputString);
			textComponent->text.setCharacterSize(textComponent->character_size);
			textComponent->text.setFillColor(textComponent->text_color);
			textComponent->text.setStyle(textComponent->style);

			sf::FloatRect bounds = textComponent->text.getLocalBounds();
			textComponent->text.setScale(scale.x, scale.y);
			textComponent->text.setPosition(position.x, position.y + yOffset);
			textComponent->text.setOrigin(bounds.width / 2, bounds.height / 2);

			GameEngine::GetInstance().window().draw(textComponent->text);
		}
	}
}

bool GameEngine::isRunning()
{
	return running_;
}

bool GameEngine::isKeyPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

// void GameEngine::sUserInput()
// {
	//sf::Event event;
	//while (window_.pollEvent(event))
	//{
	//	ImGui::SFML::ProcessEvent(event);

	//	if (event.type == sf::Event::Closed) {
	//		//window_.close();
	//	}

	//	// Update the SFML's render window_ dimensions to prevent weird sprite scaling
	//	if (event.type == sf::Event::Resized) {
	//		sf::FloatRect view(0, 0, event.size.width, event.size.height);
	//		window_.setView(sf::View(view));
	//	}

	//	// The game engine handles the following key presses:
	//	// - Enter: Toggles the game's running state
	//	// - Escape: Closes the game
	//	if (event.type == sf::Event::KeyPressed) {
	//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) 
	//		{
	//			if (running)
	//			{
	//				quit();
	//			}
	//			else
	//			{
	//				run();
	//			}
	//		}
	//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	//		{
	//			window_.close();
	//		}
	//	}
	//}
// }

void GameEngine::Interact(std::shared_ptr<Entity> collectibleEnity, std::shared_ptr<Entity> entityPair)
{

	auto collectibleComponent = collectibleEnity->getComponent<CCollectable>();
	if (collectibleComponent->is_health && entityPair->hasComponent<CHealth>())
	{ // If its health we are going to add points to the CHealth component of the entityPair
		entityPair->updateHealth(collectibleComponent->points_to_add);

		if (collectibleComponent->disappear_on_touch)
		{
			collectibleEnity->setDisabled(true);
		}
	}
	else if (entityPair->hasComponent<CText>() && entityPair->getComponent<CText>()->counter)
	{ // We are going to add score to the text comp

		entityPair->getComponent<CText>()->counter += collectibleComponent->points_to_add;

		if (collectibleComponent->disappear_on_touch)
		{
			collectibleEnity->setDisabled(true);
		}
	}

}

