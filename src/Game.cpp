#include "Game.h"

void Game::init()
{
    m_window.setFramerateLimit(60);
    sEnemySpawner();
    // spawnPlayer(); see def below
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

void Game::sEnemySpawner()
{
    auto &entityManager = EntityManager::GetInstance();
    for (int i = 0; i < 4; i++)
    {
        const auto &entity = entityManager.addEntity("Entity" + std::to_string(i));

        entity->cTransform = std::make_shared<CTransform>(Vec2(110 * i, 120 * i), Vec2(8 * i, 7 * i), 2 * i);
        entity->cName = std::make_shared<CName>("MyEntity " + std::to_string(i));
        entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(i * 11, i * 11 / 3, i * 11 / 4, 255));
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