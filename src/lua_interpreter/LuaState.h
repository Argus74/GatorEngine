#pragma once
#include "includes.h"
#include "../src/components/includes.h"
#include "../EntityManager.h"
class LuaState
{
private:
	sol::state lua_;
	sol::function script_update_;
	sol::protected_function safe_script_update_;
	sol::protected_function safe_script_start_;

public:
	//The current frame of the lua state (since the script was loaded)
	int current_frame = 0;
	//The entity that this lua state is attached to
	std::shared_ptr<Entity> attached_entity;
	LuaState(std::string lua_state_file, std::shared_ptr<Entity> attached_entity);
	sol::state& GetSolState();
	~LuaState();
	void Update();
};

