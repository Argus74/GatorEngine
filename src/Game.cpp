#include "Game.h"

void Game::init()
{
    m_window.setFramerateLimit(60);
    sEnemySpawner();
    spawnPlayer();
}

Game::Game()
{
    init();
}

void Game::update()
{
    if (Editor::state == Editor::State::Testing)
    {
        EntityManager::GetInstance().update();
        sMovement();
    }

    sRender();
}

void Game::sMovement()
{
    for (auto e : EntityManager::GetInstance().getEntities())
    {
        // TODO: update all entities with their movement components
    }

    m_player->cTransform->position.x += m_player->cTransform->velocity.x;
    m_player->cTransform->position.y += m_player->cTransform->velocity.y;
}

void Game::sRender()
{
    // TODO: implement dummy render function for testing
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

void Game::spawnPlayer()
{
    auto entity = EntityManager::GetInstance().addEntity("Player");

    entity->cTransform = std::make_shared<CTransform>(Vec2(110, 120), Vec2(8, 7), 0);
    entity->cName = std::make_shared<CName>("Player");
    entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(11, 11 / 3, 11 / 4, 255));

    // TODO: initialize player input component

    m_player = entity;
}