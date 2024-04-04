#include "LuaState.h"


LuaState::LuaState(const char* lua_state_file)
{
	//Init lua state
	l_ = luaL_newstate();
	luaL_openlibs(l_);

	int error = luaL_dofile(l_, lua_state_file);

	if (error)
	{
		std::cout << "LUA::ERROR::LUASTATE:: " << lua_tostring(l_, -1) << std::endl;
		lua_pop(this->l_, lua_gettop(this->l_));
		throw std::runtime_error("LUA::ERROR::LUASTATE::Could not open file " + std::string(lua_state_file));
	}
	
}

LuaState::~LuaState()
{
}

void LuaState::Update()
{
}
