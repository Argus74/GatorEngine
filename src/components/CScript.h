#pragma once
#include "Component.h"
#include "../Vec2.h"
#include <string>
class LuaState;
class CScript : public Component {
public:
	DECLARE_COMPONENT_NAME("Script");
	std::string script_name;
	LuaState* lua_state;
	CScript() {}
	CScript(std::string input_script_name, LuaState* input_lua_state) : script_name(input_script_name) {}

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