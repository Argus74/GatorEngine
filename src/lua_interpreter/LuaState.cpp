#include "LuaState.h"


LuaState::LuaState(const char* lua_state_file)
{
	lua_;
	lua_.open_libraries(sol::lib::base);
	lua_.script_file("script.lua");
	auto& entities = EntityManager::GetInstance().getEntities();
	lua_.set("Entities", &entities);
	script_update_ = lua_["Update"];
}

LuaState::~LuaState()
{
}

void LuaState::Update()
{
	script_update_();
}
