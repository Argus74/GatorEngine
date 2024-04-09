#pragma once
#include "includes.h"
#include "../EntityManager.h"
class LuaState
{

private:
	sol::state lua_;
	sol::function script_update_;
public:
	
	LuaState(const char* lua_state_file);
	~LuaState();
	void Update();


};

