#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include "nfd.h"

#include <iostream>
#include <string>
#include <vector>

#include "editor/Editor.h"
#include "EntityManager.h"
#include "GameEngine.h"
#include "Scene_Play.h"
#include "util/Serializable.h"
#include "util/Scene.h"
#include "lua_interpreter/LuaManager.h"
#include "lua_interpreter/LuaState.h"

// ---- SERIALIZATION WITH CLASSES EXAMPLE
// class MyClass2 : public Serializable {
// public:
//     int id;

//     void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
//         writer.StartObject();
//         writer.Key("id");
//         writer.Int(id);
//         writer.EndObject();
//     }

//     void deserialize(const rapidjson::Value& value) override {
//         id = value["id"].GetInt();
//     }
// };

// class MyClass : public Serializable {
// public:
//     int id;
//     std::string name;
//     MyClass2 obj;

//     void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
//         writer.StartObject();
//         writer.Key("id");
//         writer.Int(id);
//         writer.Key("name");
//         writer.String(name.c_str());
//         writer.Key("obj");
//         obj.serialize(writer);
//         writer.EndObject();
//     }

//     void deserialize(const rapidjson::Value& value) override {
//         id = value["id"].GetInt();
//         name = value["name"].GetString();
//         obj.deserialize(value["obj"]);
//     }
// };

int main()
{
    GameEngine& newGame = GameEngine::GetInstance();
    newGame.addEntitiesForTest();
    sf::Clock deltaClock;
    sf::RenderWindow& window = newGame.window();
    ImGui::SFML::Init(window);
    Editor editor;
    //std::shared_ptr<Scene_Old> scene = std::make_shared<Scene_Play>();
    newGame.changeScene(newGame.currentScenePath());
    //LuaState newLuaState("script.lua");

    while (window.isOpen())
    {
        newGame.update();
        ImGui::SFML::Update(window, deltaClock.restart());
        editor.Draw();
        ImGui::SFML::Render(window);
        //newLuaState.Update();
        window.display();
    }
    ImGui::SFML::Shutdown();

    // Scene scene;
    // scene.writeToJSONFile("scenes/data.scene");

    // Scene newScene;
    // EntityManager::GetInstance().reset();

    // newScene.readFromJSONFile("scenes/data.scene");

    // for (auto e : EntityManager::GetInstance().getEntities()) {
    //     e->forEachComponent([&](auto& component, int index){
    //         if (component && component->has) {
    //             std::cout << "Component: " << component->kComponentName << std::endl;
    //         }
    //     });
    // } 

    // Scene newScene;
    // EntityManager::GetInstance().reset();
    // newScene.readFromJSONFile("scenes/data.json");
    // for (auto e : EntityManager::GetInstance().getEntities()) {
    //     std::cout << "TAG: " << e->tag() << std::endl;
    // } 

    // SERIALIZATION USE EXAMPLE
    // MyClass obj;
    // obj.id = 123;
    // obj.name = "example";
    // obj.obj = MyClass2();
    // obj.obj.id = 2;

    // // Write to JSON file
    // obj.writeToJSONFile("scenes/data.json");

    // Read from JSON file
    // MyClass newObj;
    // newObj.readFromJSONFile("scenes/data.json");

    // // Display deserialized data
    // std::cout << "Deserialized object:\n";
    // std::cout << "ID: " << newObj.id << ", Name: " << newObj.name << std::endl;
    // std::cout << "Obj - ID:" << newObj.obj.id << std::endl;

    return 0;
}
