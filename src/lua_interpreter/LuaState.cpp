#include "LuaState.h"


LuaState::LuaState(const char* lua_state_file)
{
	lua_;
	lua_.open_libraries(sol::lib::base);
	lua_.script_file("script.lua");
	lua_.set("Entities", std::ref(EntityManager::GetInstance().getEntities()));
	lua_.set_function("Entity", &EntityManager::addEntity, std::ref(EntityManager::GetInstance()));
	script_update_ = lua_["Update"];
}

LuaState::~LuaState()
{
}

void LuaState::Update()
{
	//lua_.set("Entities", &EntityManager::GetInstance().getEntities());
	script_update_();
}
