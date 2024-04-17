#pragma once
#include "Scene.h"
#include "AssetManager.h"
#include "./Editor/Editor.h"
#include "lua_interpreter/LuaState.h"
#include <memory>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{

protected:
	sf::RenderWindow m_window = sf::RenderWindow(sf::VideoMode(1366, 768), "Gator Engine");
	AssetManager& m_assets = AssetManager::GetInstance();
	std::string	m_currentScene;
	SceneMap m_sceneMap;
	size_t m_simulationSpeed = 1;
	bool m_running = true;
	bool initialized_ = false;
	void init(const std::string& path);
	void sUserInput();
	std::shared_ptr<Scene> currentScene;
	std::map<sf::Keyboard::Key, bool> m_keyMap;

private:
	GameEngine();
public:
	sf::Keyboard::Key last_key_pressed;
	std::map <std::shared_ptr<Entity>, std::shared_ptr<LuaState>> lua_states;
	sf::Clock* deltaClock;
	void update();
	static GameEngine& GetInstance();
	void ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
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