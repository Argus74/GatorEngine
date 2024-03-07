#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "./Editor/Editor.h"
class Game
{
private:
	std::shared_ptr<Entity> m_player;
	bool m_paused;
	int m_currentFrame;

	void init();
	void spawnPlayer();

public:
	sf::RenderWindow m_window = sf::RenderWindow(sf::VideoMode(1366, 768), "Gator Engine");

	Game();
	// Game(std::shared_ptr<Entity> player); - mock constructor for initializing player
	void update();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void sUserInput();
	void sAnimation();
	void sMovement();
};