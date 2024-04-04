#pragma once
#include "util/Scene.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "./editor/Editor.h"
#include <memory>

// typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{

// protected:

private:
	GameEngine();
	sf::RenderWindow window_ = sf::RenderWindow(sf::VideoMode(1366, 768), "Gator Engine");

	// std::string m_currentScene;
	// SceneMap scene_map_;
	size_t simulation_speed_ = 1;
	bool running_ = true;
	bool initialized_ = false;
	size_t current_frame_ = 0;	

	void init(const std::string& path);

	void sUserInput();
	void sMovement();
	void sPhysics();
	void sCollision();
	void sAnimation();
	void sRender();

	// std::shared_ptr<Scene> current_scene_;
	std::map<sf::Keyboard::Key, bool> key_map_;

public:
	// sf::Clock* delta_clock;
	void addEntitiesForTest();
	void update();
	static GameEngine& GetInstance();
	// void ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
	void quit();
	void run();
	sf::RenderWindow& window();
	AssetManager& assets();
	bool isRunning();
	bool isKeyPressed(sf::Keyboard::Key key);

	//Delete copy and move constructors
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;
	GameEngine(GameEngine&&) = delete;
	GameEngine& operator=(GameEngine&&) = delete;
};