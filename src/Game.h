#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "./Editor/Editor.h"
#include "Physics.h"
class Game {
private:
	Entity m_player;
	bool m_paused;
	bool m_running;
	
	void init();
public:
	sf::RenderWindow m_window = sf::RenderWindow(sf::VideoMode(1366, 768), "Gator Engine");

	Game();
	void run();
	void update();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	//We will implement sPhysics() once entities can have physics components (Velocity, Acceleration, etc)
	void sPhysics();
	void sUserInput();
	void sAnimation();
	void sMovement();
	

};