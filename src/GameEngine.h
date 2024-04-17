#pragma once
#include "util/Scene.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "./editor/Editor.h"
#include "util/Serializable.h"
#include <memory>

// typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine : public Serializable
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
	std::string current_scene_path_;	

	void init();

	void sUserInput();
	void sTouchTrigger();
	void sMovement();
	void sPhysics();
	void sCollision();
	void sBackground();
	// void sAnimation();
	void sRender();

	// std::shared_ptr<Scene> current_scene_;
	std::map<sf::Keyboard::Key, bool> key_map_;

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
		writer.StartObject();
		writer.Key("lastScene");
		writer.String(current_scene_path_.c_str());	
		writer.EndObject();
	}

	void deserialize(const rapidjson::Value& value) override {
		current_scene_path_ = value["lastScene"].GetString();
	}

public:
	// sf::Clock* delta_clock;
	void addEntitiesForTest();
	void changeScene(const std::string& path);
	void saveScene(const std::string& path);
	void update();
	static GameEngine& GetInstance();
	// void ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
	void quit();
	void run();
	sf::RenderWindow& window();
	AssetManager& assets();
	bool isRunning();
	bool isKeyPressed(sf::Keyboard::Key key);
	const std::string& currentScenePath() const { return current_scene_path_; };

	//Delete copy and move constructors
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;
	GameEngine(GameEngine&&) = delete;
	GameEngine& operator=(GameEngine&&) = delete;
};