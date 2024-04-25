#include "GameEngine.h"

#include <filesystem>

GameEngine::GameEngine() {}

GameEngine& GameEngine::GetInstance() {
    // TODO: insert return statement here
    static GameEngine instance_;

    if (!instance_.initialized_) {

        instance_.init();
        instance_.initialized_ = true;
    }
    return instance_;
}

void GameEngine::quit() {
    running_ = false;
}

void GameEngine::run() {
    running_ = true;
}

sf::RenderWindow& GameEngine::window() {
    return window_;
}

AssetManager& GameEngine::assets() {
    return AssetManager::GetInstance();
}

void GameEngine::init() {
    // Call the AssetManager singleton for the first time to initialize all starting assets
    AssetManager::GetInstance();

    // Load the last scene from the last-scene.json file, if it exists
    if (!readFromJSONFile("last-scene.json")) {
        current_scene_path_ = "scenes/City.scene";
    }

    window_.setFramerateLimit(60);
}

void GameEngine::changeScene(const std::string& path) {
    EntityManager::GetInstance().reset();
    Editor::state = Editor::State::Selecting;
    Editor::active_entity_ = nullptr;
    GatorPhysics::GetInstance().clearBodies();
    Scene scene;
    scene.readFromJSONFile(path);
    current_scene_path_ = path;
    writeToJSONFile("last-scene.json");
}

void GameEngine::saveScene(const std::string& path) {
    Scene scene;
    scene.writeToJSONFile(path);
    current_scene_path_ = path;
    writeToJSONFile("last-scene.json");
}

void GameEngine::update() {
    EntityManager::GetInstance().update();

    sUserInput();
    if (Editor::state == Editor::State::Testing) {
        auto& entities_to_bodies = GatorPhysics::GetInstance().GetEntityToBodies();
        sTouchTrigger();
        sScripts();
        sMovement();
        sPhysics();
        sCollision();
    }

    sBackground();
    sRender();
    if (Editor::state == Editor::State::Testing) {
        //sRenderColliders();
    }
    sUI();

    deltaClock.restart();
}

void GameEngine::sUserInput() {
    // Clear the bus from previous frame's input
    ActionBus::GetInstance().Clear();
    sf::Event event;
    while (window_.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        // Update the SFML's render window dimensions to prevent weird sprite scaling
        if (event.type == sf::Event::Resized) {
            sf::FloatRect view(0, 0, event.size.width, event.size.height);
            window_.setView(sf::View(view));
        }

        // Editor-specific hotkeys
        if (Editor::active_entity_ && Editor::state != Editor::State::Testing) {
            // Ctrl+D to copy active entity
            if (event.type == sf::Event::KeyPressed && event.key.control &&
                event.key.code == sf::Keyboard::D) {
                EntityManager::GetInstance().cloneEntity(Editor::active_entity_);
            }

            // Ctrl+X to delete active entity
            if (event.type == sf::Event::KeyPressed && event.key.control &&
                event.key.code == sf::Keyboard::X) {
                EntityManager::GetInstance().removeEntity(Editor::active_entity_);
            }

            // Ctrl+Z hotkey does not exist. Good luck o7
        }

        // Detect mouse clicks
        if (event.type == sf::Event::MouseButtonPressed) {
            for (auto& entity : EntityManager::GetInstance().getEntities()) {
                if (!entity->hasComponent<CUserInput>() || entity->isDisabled())
                    continue;

                auto& mouseMap = entity->getComponent<CUserInput>()->mouse_map;
                for (auto& actionKeys : mouseMap) {
                    auto button = event.mouseButton.button;
                    if (actionKeys.first == button) {
                        ActionBus::GetInstance().Dispatch(entity, actionKeys.second);
                    }
                }
            }
        }
    }

    // Detect keypresses using IsKeyPressed API for continuous responses
    for (auto& entity : EntityManager::GetInstance().getEntities()) {
        if (!entity->hasComponent<CUserInput>() || entity->isDisabled())
            continue;

        auto& inputMap = entity->getComponent<CUserInput>()->key_map;
        for (auto& actionKeys : inputMap) {
            if (sf::Keyboard::isKeyPressed(actionKeys.first)) {
                ActionBus::GetInstance().Dispatch(entity, actionKeys.second);
            }
        }
    }
}

void GameEngine::sTouchTrigger() {
    auto& entities = EntityManager::GetInstance().getEntities();
    for (auto& entity : entities) {
        // Skip entities without a touch trigger component, or that are disabled
        if (!entity->hasComponent<CTouchTrigger>() || entity->isDisabled())
            continue;
        auto touchTrigger = entity->getComponent<CTouchTrigger>();
        auto transform = entity->getComponent<CTransform>();
        sf::FloatRect triggerRect;
        triggerRect.width = touchTrigger->trigger_size.x;
        triggerRect.height = touchTrigger->trigger_size.y;
        triggerRect.left = transform->position.y - (triggerRect.height / 2);
        triggerRect.top = transform->position.x - (triggerRect.width / 2);

        // If has touch trigger, check if it is touching any other entity
        for (auto& entityTouched : entities) {
            // Skip entities without the tag we're caring about
            if (touchTrigger->tag != entityTouched->getComponent<CInformation>()->tag ||
                entity->isDisabled() || entityTouched->isDisabled())
                continue;

            // Check if the entity is touching the entity with the touch trigger
            auto entityTouchedRect =
                entityTouched->GetRect(5);  // Add leeway to the entity touched rect
            if (triggerRect.intersects(entityTouchedRect)) {
                // Only proceeding with an action if their is an collectable component attached, and its nots a health
                if (touchTrigger->action == UpdateCollectible) {
                    if (entity->hasComponent<CCollectable>() &&
                        !entity->getComponent<CCollectable>()->is_health) {
                        // We are going to be updating not the entity that is touched, but rather the Text correlated to the collectable
                        auto collectableEnityText = EntityManager::GetInstance().getEntityByName(
                            entity->getComponent<CCollectable>()->text_entity_name);
                        Interact(entity, collectableEnityText);
                    }
                } else if (touchTrigger->action == UpdateHealth) {
                    if (entity->hasComponent<CCollectable>() &&
                        entity->getComponent<CCollectable>()->is_health) {
                        Interact(entity, entityTouched);
                    }
                } else if (touchTrigger->action == GiveJump) {
                    ActionBus::GetInstance().Dispatch(entityTouched, Jump);
                }
            }
        }
    }
}

void GameEngine::sScripts() {
    //First, check if there are any entities that have been given a script component. If so,
    //add them to map of entities to lua states
    for (auto entity : EntityManager::GetInstance().getEntities()) {
        if (entity->hasComponent<CScript>() && !lua_states[entity]) { // TODO: Check if script name was changed?
            //Verify that the script name that the user typed in the editor is valid
            std::string scriptPath = "scripts/" + entity->getComponent<CScript>()->script_name;

            // Verify that the script name that was deserialized is valid (editor will catch all other invalid names)
            if (scriptPath.substr(scriptPath.find_last_of(".") + 1) != "lua") {
                std::cerr << "Invalid script name: " << scriptPath << std::endl;
                continue;
            }

            // If script not found (usually because the user deleted it, its a saved scene, and this is the first time the engine is running),
            // then we'll create a default script for the user
            if (!std::filesystem::exists(std::filesystem::path(scriptPath))) {
                std::ofstream default_script(scriptPath);
                default_script << "-- Default script for "
                               << entity->getComponent<CScript>()->script_name << std::endl;
                default_script << "function Update()\n";
                default_script << "end\n";
                default_script.close();
            }

            std::shared_ptr<LuaState> new_lua_state =
                std::make_shared<LuaState>(scriptPath, entity);
            lua_states[entity] = new_lua_state;
            entity->getComponent<CScript>()->lua_state = new_lua_state.get();
        }
        else
        {
            //If the entity already has a script component, check if the name has been changed
            // by looking at stored name in lua_states (minus its prepended "scripts/" string)
            if (entity->hasComponent<CScript>() && lua_states[entity] &&
                entity->getComponent<CScript>()->script_name != lua_states[entity]->name.substr(8)) {
				//If the name has been changed, remove the old lua state and create a new one
				lua_states[entity].reset();
				std::string scriptPath = "scripts/" + entity->getComponent<CScript>()->script_name;

                if (scriptPath.substr(scriptPath.find_last_of(".") + 1) == "lua") {
                    std::ofstream default_script(scriptPath);
                    default_script << "-- Default script for "
                                   << entity->getComponent<CScript>()->script_name << std::endl;
                    default_script << "function update()\n";
                    default_script << "end\n";
                    default_script.close();
                } else {
                    std::cerr << "Invalid script name: " << scriptPath << std::endl;
                    continue;
                }

				std::shared_ptr<LuaState> new_lua_state =
					std::make_shared<LuaState>(scriptPath, entity);
				lua_states[entity] = new_lua_state;
				entity->getComponent<CScript>()->lua_state = new_lua_state.get();
			}

        }
    }

    //For each lua state, update the script
    for (auto node : lua_states) {
        //For each lua state, update the script
        if (node.second) {
            node.second->Update();
        }
    }
}

void GameEngine::sMovement() {
    for (auto entity : EntityManager::GetInstance().getEntities()) {
        // Skip unrelated entities
        if (!entity->hasComponent<CTransform>() || entity->isDisabled())
            continue;

        auto transform = entity->getComponent<CTransform>();
        auto character = entity->getComponent<CCharacter>();
        auto rigidBody = entity->getComponent<CRigidBody>();
        // Handle movement
        // Start using the constant velocity (idk why anyone would ever set it to non-0 but just in case they do)
        Vec2 speed = transform->velocity;
        float dt = deltaClock.getElapsedTime().asMilliseconds() / 9; // delta time for more consistent movement
        float charSpeed = character ? character->speed * dt : 5.0 * dt;

        if (ActionBus::GetInstance().Received(entity, MoveRight))
            speed.x += charSpeed;
        if (ActionBus::GetInstance().Received(entity, MoveLeft))
            speed.x -= charSpeed;
        if (ActionBus::GetInstance().Received(entity, MoveUp))
            speed.y -= charSpeed;
        if (ActionBus::GetInstance().Received(entity, MoveDown))
            speed.y += charSpeed;

        // Update the position based on the velocity
        transform->position = transform->position + speed;

        // Use the RigidBody to process physics movements
        auto it = GatorPhysics::GetInstance().GetEntityToBodies().find(entity.get());

        if (!entity->hasComponent<CRigidBody>() ||
            it == GatorPhysics::GetInstance().GetEntityToBodies().end())
            continue;
        b2Body* body = it->second;

        // Handle jumps
        Vec2 jumpPower = character ? character->jump_force : Vec2(0, 10);
        if (ActionBus::GetInstance().Received(entity, Jump) && rigidBody->is_grounded) {
            body->ApplyLinearImpulseToCenter(b2Vec2(jumpPower.x, jumpPower.y), true);
            rigidBody->is_grounded = false;
        }
    }
}

void GameEngine::sPhysics() {}

void GameEngine::sCollision() {
    //First check if any new entities have a new rigid body component and
    // have not been added to the physics world
    auto listOfEntities = EntityManager::GetInstance().getEntities();
    for (auto entity : EntityManager::GetInstance().getEntities()) {
        if (entity->hasComponent<CRigidBody>() && !entity->isDisabled()) {
            auto rigidBodyComponent = entity->getComponent<CRigidBody>();
            std::map<Entity*, b2Body*>& entity_to_bodies_ =
                GatorPhysics::GetInstance().GetEntityToBodies();
            //If the entity is not in the physics world, add it
            auto result = entity_to_bodies_.find(entity.get());
            if (result == entity_to_bodies_.end()) {
                GatorPhysics::GetInstance().createBody(entity.get(),
                                                       rigidBodyComponent->static_body);
            }
        }
    }

    GatorPhysics::GetInstance().update();
}

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

void GameEngine::sRender() {
    auto& entityManager = EntityManager::GetInstance();

    std::vector<std::shared_ptr<Entity>>& entityList = entityManager.getEntitiesRenderingList();

    for (auto& entity :
         entityList) {  // Looping through entity list and drawing the sprites to the render window.
        if (entity->hasComponent<CSprite>() && !entity->isDisabled()) {
            auto transformComponent = entity->getComponent<CTransform>();
            Vec2 scale = transformComponent->scale;
            Vec2 position =
                transformComponent
                    ->position;  // getting the scale and positioning from the transform component in order to render sprite at proper spot
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
                GameEngine::GetInstance().window().draw(spriteComponent->sprite);
        }

        if (entity->hasComponent<CAnimation>() && !entity->isDisabled()) {
            auto transformComponent = entity->getComponent<CTransform>();
            Vec2 scale = transformComponent->scale;
            Vec2 position =
                transformComponent
                    ->position;  // getting the scale and positioning from the transform component in order to render sprite at proper spot
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
            GameEngine::GetInstance().window().draw(sprite);

            if (animationComponent->play_animation || Editor::state == Editor::Testing)
                animationComponent->update();
        }
    }
}

void GameEngine::sUI() {
    auto& entityManager = EntityManager::GetInstance();

    std::vector<std::shared_ptr<Entity>>& entityList =
        entityManager.getUIRenderingList();  // We only iterate through the UI rendering list

    for (auto& entity : entityList) {

        if (entity->hasComponent<CHealth>() && entity->getComponent<CHealth>()->draw_health &&
            !entity->isDisabled()) {  // Health Bar
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
                Vec2 position = entity->getComponent<CTransform>()->position +
                                healthComponent->health_bar_offset;
                backHealth.setPosition(position.x, position.y + yOffset);
                backHealth.setScale(scale.x, scale.y);

                frontHealth.setPosition(position.x, position.y + yOffset);
                frontHealth.setScale(scale.x, scale.y);
            } else {
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

        if (entity->hasComponent<CText>() && !entity->isDisabled()) {
            auto transformComponent = entity->getComponent<CTransform>();  // Transform related
            Vec2 scale = transformComponent->scale;
            Vec2 position = transformComponent->position;
            float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;

            auto textComponent = entity->getComponent<CText>();  // Setting the properties of the text

            std::string outputString = textComponent->message;
            if (textComponent->is_counter) {
                int numberToAdd = textComponent->counter;
                outputString += std::to_string(numberToAdd);
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

bool GameEngine::isRunning() {
    return running_;
}

void GameEngine::sRenderColliders() {
    auto& entityManager = EntityManager::GetInstance();

    std::vector<std::shared_ptr<Entity>>& entityList = entityManager.getEntities();

    for (auto& entity :
         entityList) {  // Looping through entity list and drawing the sprites to the render window.
        if (entity->hasComponent<CRigidBody>() && !entity->isDisabled()) {
            auto rigidBodyComponent = entity->getComponent<CRigidBody>();
            b2Vec2 position = rigidBodyComponent->body->GetPosition();
            //These sizes are half widths and half heights
            b2Vec2 size = rigidBodyComponent->body->GetFixtureList()->GetAABB(0).GetExtents();
            float xScale = entity->getComponent<CTransform>()->scale.x;
            float yScale = entity->getComponent<CTransform>()->scale.y;
            float entityWidth = size.x * 2 * GatorPhysics::GetInstance().getScale();
            float entityHeight = size.y * 2 * GatorPhysics::GetInstance().getScale();

            float yOffset = ImGui::GetMainViewport()->Size.y * .2 + 20;
            float worldY = GameEngine::GetInstance().window().getSize().y;
            float entityY = ((position.y * GatorPhysics::GetInstance().getScale()) - worldY) * -1;
            float entityX = position.x * GatorPhysics::GetInstance().getScale();
        }
    }
}

// Note: entityPair represents the entity to affect: for health collectables, that is the entity who touched; for all others, it's the counter UI
void GameEngine::Interact(std::shared_ptr<Entity> collectibleEnity,
                          std::shared_ptr<Entity> entityPair) {
    auto collectibleComponent = collectibleEnity->getComponent<CCollectable>();
    collectibleComponent->touched = true;
    // If its health we are going to add points to the CHealth component of the entityPair
    if (collectibleComponent->is_health && entityPair && entityPair->hasComponent<CHealth>()) {
        entityPair->updateHealth(collectibleComponent->points_to_add);

        collectibleComponent->touched = true;

        if (collectibleComponent->disappear_on_touch) {
            collectibleEnity->setDisabled(true);
        }
    // Else, interact with a collectable per usual
    // We are going to add score to the text comp
    } else {
        if (entityPair && entityPair->hasComponent<CText>() && entityPair->getComponent<CText>()->is_counter)
            entityPair->getComponent<CText>()->counter += collectibleComponent->points_to_add;

        if (collectibleComponent->disappear_on_touch) {
            collectibleEnity->setDisabled(true);
        }
    }
}
