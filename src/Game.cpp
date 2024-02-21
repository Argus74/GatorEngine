#include "Game.h"

void Game::init()
{   
    m_window.setFramerateLimit(60);
    sEnemySpawner();
}

Game::Game()
{
    init();
}

void Game::update()
{
    EntityManager::GetInstance().update();
}

void Game::sEnemySpawner()
{
    auto& entityManager = EntityManager::GetInstance();
    for (int i = 0; i < 4; i++)
    {
        const auto& entity = entityManager.addEntity("Entity" + std::to_string(i));
        entity->cTransform = std::make_shared<CTransform>(Vec2(110 * i, 120 * i), Vec2(8 * i, 7 * i), 2 * i);
        entity->cName = std::make_shared<CName>("MyEntity " + std::to_string(i));
        entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(i * 11, i * 11 / 3, i * 11 / 4, 255));
    }
}
