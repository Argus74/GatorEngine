#pragma once

#include <memory>

#include "./editor/Editor.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "lua_interpreter/LuaState.h"
#include "util/Scene.h"
#include "util/Serializable.h"

class GameEngine : public Serializable {

 private:
    GameEngine();

    sf::RenderWindow window_ = sf::RenderWindow(sf::VideoMode(1366, 768), "Gator Engine");

    size_t simulation_speed_ = 1;
    bool running_ = true;
    bool initialized_ = false;
    std::string current_scene_path_ = "scenes/City.scene";

    void init();

    void sUserInput();
    void sTouchTrigger();
    void sScripts();
    void sMovement();
    void sPhysics();
    void sCollision();
    void sBackground();
    void sRenderColliders();
    void sRender();
    void sUI();

    void Interact(std::shared_ptr<Entity> collectibleEnity,
                  std::shared_ptr<Entity> entityPair);  // Used for sTouchTrigger()

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
    std::map<std::shared_ptr<Entity>, std::shared_ptr<LuaState>> lua_states;
    sf::Clock deltaClock;
    void changeScene(const std::string& path);
    void saveScene(const std::string& path);
    void update();
    static GameEngine& GetInstance();

    void quit();
    void run();
    sf::RenderWindow& window();
    AssetManager& assets();
    bool isRunning();

    const std::string& currentScenePath() const { return current_scene_path_; };

    //Delete copy and move constructors
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
    GameEngine(GameEngine&&) = delete;
    GameEngine& operator=(GameEngine&&) = delete;
};