/**
 * File: CScript.h
 *
 * The CScript class manages the scripting components that can be attached to game entities.
 *
 * This component is designed to link entities with specific scripts, allowing for complex behaviors defined in Lua or similar scripting languages.
 * It facilitates dynamic interactions within the game world by embedding Lua scripts that handle game logic, event responses, and other functionalities.
 */


#pragma once

#include <string>

#include "../Vec2.h"
#include "Component.h"

class LuaState;
class CScript : public Component {
 public:
    DECLARE_COMPONENT_NAME("Script");
    std::string script_name;
    LuaState* lua_state;
    CScript() {}
    CScript(std::string input_script_name, LuaState* input_lua_state)
        : script_name(input_script_name) {}

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("name");
        writer.String(script_name.c_str());
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        script_name = value["name"].GetString();
    }
};