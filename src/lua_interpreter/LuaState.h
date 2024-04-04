#pragma once
#include "includes.h"

class LuaState
{

private:
	lua_State* l_;

public:
	LuaState(const char* lua_state_file);
	~LuaState();
	void Update();


};

