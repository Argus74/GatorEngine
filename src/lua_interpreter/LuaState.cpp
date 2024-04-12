#include "LuaState.h"


LuaState::LuaState(const char* lua_state_file)
{
	lua_;
	lua_.open_libraries(sol::lib::base);
	lua_.script_file("script.lua");
	/*lua_.new_usertype<Entity>("Entity", sol::no_constructor,
		"isAlive", &Entity::isAlive,
		"hasComponent", &Entity::hasComponent<CName>,
		"addComponent", &Entity::addComponent<CName,>);*/

	lua_.new_usertype<EntityManager>("EntityManager", sol::no_constructor,
				"AddEntity", &EntityManager::addEntity,
				"GetEntities", &EntityManager::getEntities);
	lua_.set("EntityManager", std::ref(EntityManager::GetInstance()));

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
