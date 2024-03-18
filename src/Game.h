#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "./Editor/Editor.h"
#include "GatorPhysics.h"
class Game {
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
	void sEnemySpawner();
	void sCollision();
	//We will implement sPhysics() once entities can have physics components (Velocity, Acceleration, etc)
	//void sPhysics();
	void sUserInput();
	void sAnimation();
	void sMovement();
	void sRender();
};