#include "Game.h"
#include "ActionBus.h"

void Game::init()
{

    // Just temporary sprites for testing once, we get a file system going we will do this somewhere else
    auto &assetManager = AssetManager::GetInstance();
    assetManager.AddTexture("DefaultSprite", "assets/DefaultSprite.png");
    assetManager.AddTexture("DefaultAnimationTexture", "assets/DefaultAnimation.png");
    assetManager.AddTexture("RunningAnimationTexture", "assets/RunningAnimation.png");
    Animation &ani = Animation("DefaultAnimation", assetManager.GetTexture("DefaultAnimationTexture"), 11, 1);
    assetManager.AddAnimation("DefaultAnimation", ani);
    Animation &ani2 = Animation("RunningAnimation", assetManager.GetTexture("RunningAnimationTexture"), 11, 1);
    assetManager.AddAnimation("RunningAnimation", ani2);
    // 123

    m_window.setFramerateLimit(60);
    sEnemySpawner();
#include "ActionBus.h"

    void Game::init()
    {
        // Just temporary sprites for testing once, we get a file system going we will do this somewhere else
        auto &assetManager = AssetManager::GetInstance();
        assetManager.AddTexture("DefaultSprite", "assets/DefaultSprite.png");
        assetManager.AddTexture("DefaultAnimationTexture", "assets/DefaultAnimation.png");
        assetManager.AddTexture("RunningAnimationTexture", "assets/RunningAnimation.png");
        Animation &ani = Animation("DefaultAnimation", assetManager.GetTexture("DefaultAnimationTexture"), 11, 1);
        assetManager.AddAnimation("DefaultAnimation", ani);
        Animation &ani2 = Animation("RunningAnimation", assetManager.GetTexture("RunningAnimationTexture"), 11, 1);
        assetManager.AddAnimation("RunningAnimation", ani2);
        // 123

        m_window.setFramerateLimit(60);
        sEnemySpawner();
    }

    Game::Game()
    {
        init();
    }

    /*

    Mock constructor for initializing player

    Game::Game(std::shared_ptr<Entity> player)
    {
        init();
        this->m_player = player;
    }
    */

    void Game::update()
    {
        EntityManager::GetInstance().update();
        EntityManager::GetInstance().update();
        // GLUING NOTE: Call cUserInput() before rest of systems. Also, only call it if the game is playing (not paused).
        sUserInput();
        sSprite();
        sAnimation();
    }

    void Game::sUserInput()
    {
        // Clear the bus from previous frame's input
        ActionBus::GetInstance().Clear();

        sf::Event event;
        while (m_window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }

            // Update the SFML's render window dimensions to prevent weird sprite scaling
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect view(0, 0, event.size.width, event.size.height);
                m_window.setView(sf::View(view));
            }

            // Lambda to process key or mouse events
            auto processInputEvent = [](const sf::Event &event, const sf::Event::EventType &eventType)
            {
                auto &entities = EntityManager::GetInstance().getEntities();
                for (auto &entity : entities)
                {

                    // Skip entities without a cUserInput component
                    if (!entity->cUserInput)
                        continue;

                    auto findAndDispatch = [entity](auto &inputMap, const auto &eventButton)
                    {
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
                        auto &inputMap = entity->cUserInput->keyMap;
                        findAndDispatch(inputMap, event.key.code);
                    }
                    else
                    {
                        auto &inputMap = entity->cUserInput->mouseMap;
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
            sMovement();
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

    void Game::sMovement()
    {
        for (auto e : EntityManager::GetInstance().getEntities())
        {
            // TODO: update all entities with their movement components and behaviors
        }

        // TODO: update player movement based on input & use velocity in CTransform
    }
}
}

void Game::sEnemySpawner()
{
    auto &entityManager = EntityManager::GetInstance();
    auto &entityManager = EntityManager::GetInstance();
    for (int i = 0; i < 4; i++)
    {
        const auto &entity = entityManager.addEntity("Entity" + std::to_string(i));
        entity->cTransform = std::make_shared<CTransform>(Vec2(110 * i, 120 * i), Vec2(1, 1), 2 * i);
        entity->cName = std::make_shared<CName>("MyEntity " + std::to_string(i));
        entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(i * 11, i * 11 / 3, i * 11 / 4, 255));
        if (i == 1)
        {
            entity->addComponent<CSprite>();
        }
        if (i == 2)
        {
            entity->addComponent<CAnimation>();
        }
        if (i == 3)
        {
            entity->addComponent<CAnimation>();
        }
    }
}

void Game::sCollision()
{
    Physics::GetInstance()->update();
}

void Game::sCollision()
{
    Physics::GetInstance()->update();
}

void Game::sAnimation()
{
    // Need to add GetComponent, AddComponent templates to entity.
    auto &entityManager = EntityManager::GetInstance();
    std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntities();

    for (auto &entity : entityList)
    {
        if (entity->hasComponent<CAnimation>())
        {
            auto &transformComponent = entity->cTransform;
            Vec2 scale = transformComponent->scale;
            Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
            auto &animationComponent = entity->getComponent<CAnimation>();
            animationComponent->changeSpeed();

            sf::Sprite sprite(animationComponent->animation_.sprite_);
            sprite.setPosition(position.x, position.y + 150);
            sprite.setScale(scale.x, scale.y);

            m_window.draw(sprite);
            animationComponent->update();
        }
    }
}

void Game::sSprite()
{
    auto &entityManager = EntityManager::GetInstance();

    std::vector<std::shared_ptr<Entity>> &entityList = entityManager.getEntities();

    for (auto &entity : entityList)
    { // Looping through entity list and drawing the sprites to the render window.
        if (entity->hasComponent<CSprite>())
        {
            auto &transformComponent = entity->cTransform;
            Vec2 scale = transformComponent->scale;
            Vec2 position = transformComponent->position; // getting the scale and positioning from the transform component in order to render sprite at proper spot
            auto &spriteComponent = entity->getComponent<CSprite>();

            // spriteComponent->setPosition(position);
            // spriteComponent->setScale(scale);

            sf::Sprite sprite(spriteComponent->texture_);
            sprite.setPosition(position.x, position.y + 150);
            sprite.setScale(scale.x, scale.y);

            if (spriteComponent->drawSprite_)
                m_window.draw(sprite);
        }
    }
}

// This function should probably just be used for our demo if it's still needed
// Creates and initializes a player
// Maybe have the option for the user to select a game object as a player? Since multiple game objects shouldn't be input usable
void Game::spawnPlayer()
{
    auto entity = EntityManager::GetInstance().addEntity("Player");

    entity->cTransform = std::make_shared<CTransform>(Vec2(110, 700), Vec2(80, 70), 0);
    entity->cName = std::make_shared<CName>("Player");
    entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color::Red);

    // TODO: maybe add CInput component here for testing/demo, would need to initialize player somewhere in init/constructor

    m_player = entity;
}
