#include "LuaState.h"

int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
	// L is the lua state, which you can wrap in a state_view if necessary
	// maybe_exception will contain exception, if it exists
	// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
	std::cout << "An exception occurred in a function, here's what it says ";
	if (maybe_exception) {
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	}
	else {
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
		std::cout << std::endl;
	}

	// you must push 1 element onto the stack to be
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
	// so we push a single string (in our case, the description of the error)
	return sol::stack::push(L, description);
}

void will_throw() {
	throw std::runtime_error("oh no not an exception!!!");
}


inline void my_panic(sol::optional<std::string> maybe_msg) {
	std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
	if (maybe_msg) {
		const std::string& msg = maybe_msg.value();
		std::cerr << "\terror message: " << msg << std::endl;
	}
	// When this function exits, Lua will exhibit default behavior and abort()
}

LuaState::LuaState(std::string lua_state_file, std::shared_ptr<Entity> attached_entity)
{
	this->attached_entity = attached_entity;
	lua_.set_panic(sol::c_call<decltype(&my_panic), &my_panic>);
	lua_.open_libraries(
		sol::lib::base, 
		sol::lib::package, 
		sol::lib::string, 
		sol::lib::os, 
		sol::lib::math, 
		sol::lib::table, 
		sol::lib::debug, 
		sol::lib::io, 
		sol::lib::jit
	);
	
	lua_.set_exception_handler(&my_exception_handler);
	lua_.set_function("will_throw", &will_throw);
	lua_.unsafe_script_file(lua_state_file);

	sol::usertype<CName> name = lua_.new_usertype<CName>("CName");
	name["Name"] = &CName::name;
	sol::usertype<Vec2> vec2 = lua_.new_usertype<Vec2>("Vec2", sol::constructors<Vec2(), Vec2(float, float)>());
	vec2["X"] = &Vec2::x;
	vec2["Y"] = &Vec2::y;

	sol::usertype<CInformation> information = lua_.new_usertype<CInformation>("CInformation");
	information["Layer"] = &CInformation::layer;
	information["Tag"] = &CInformation::tag;

	sol::usertype<CTransform> cTransform = lua_.new_usertype<CTransform>("CTransform");
	cTransform["Position"] = &CTransform::position;
	cTransform["Scale"] = &CTransform::scale;
	cTransform["Angle"] = &CTransform::angle;
	cTransform["Velocity"] = &CTransform::velocity;

	sol::usertype<CCharacter> cCharacter = lua_.new_usertype<CCharacter>("CCharacter");
	cCharacter["Speed"] = &CCharacter::speed;
	cCharacter["JumpForce"] = &CCharacter::jump_force;

	sol::usertype<CHealth> cHealth = lua_.new_usertype<CHealth>("CHealth");
	cHealth["CurrentHealth"] = &CHealth::currentHealth_;
	cHealth["HealthTotal"] = &CHealth::healthTotal_;
	cHealth["DrawHealth"] = &CHealth::drawHealth_;
	cHealth["FollowEntity"] = &CHealth::followEntity;
	cHealth["RespawnCharacter"] = &CHealth::respawnCharacter_;
	cHealth["RespawnPosition"] = &CHealth::respawnPosition_;

	sol::usertype<CText> cText = lua_.new_usertype<CText>("CText");
	cText["Text"] = &CText::text_;


	sol::usertype<Entity> entity = lua_.new_usertype<Entity>("Entity");
	entity["IsAlive"] = &Entity::isAlive;
	entity["AddComponent"] = static_cast<void(Entity::*)(std::string)>(&Entity::addComponent);
	entity["HasComponent"] = static_cast<bool(Entity::*)(std::string)>(&Entity::hasComponent);
	entity["GetCName"] = static_cast<std::shared_ptr<CName>&(Entity::*)()>(&Entity::getComponent<CName>);
	entity["GetCInformation"] = static_cast<std::shared_ptr<CInformation>&(Entity::*)()>(&Entity::getComponent<CInformation>);
	entity["GetCTransform"] = static_cast<std::shared_ptr<CTransform>&(Entity::*)()>(&Entity::getComponent<CTransform>);
	//entity["GetCCharacter"] = static_cast<std::shared_ptr<CCharacter>&(Entity::*)()>(&Entity::getComponent<CCharacter>);


	

	lua_.set("AttachedEntity", attached_entity);
	//lua_["Entity"]["GetCSprite"] = static_cast<std::shared_ptr<CSprite>&(Entity::*)(std::string)>(&Entity::getComponent<CSprite>);
	//lua_["Entity"]["GetCShape"] = static_cast<std::shared_ptr<CShape>&(Entity::*)(std::string)>(&Entity::getComponent<CShape>);
	//lua_["Entity"]["GetCUserInput"] = static_cast<std::shared_ptr<CUserInput>&(Entity::*)(std::string)>(&Entity::getComponent<CUserInput>);

	
	/*lua_.new_usertype<CSprite>("CSprite", 
		"LoadFromAssetManager", static_cast<bool(CSprite::*)()>(&CSprite::loadFromAssetManager),
		"SetPosition", static_cast<void(CSprite::*)(float, float)>(&CSprite::setPosition));*/
	/*lua_.new_usertype<CShape>("CShape");
	lua_.new_usertype<CUserInput>("CUserInput");
	lua_.new_usertype<CAnimation>("CAnimation");
	lua_.new_usertype<CRigidBody>("CRigidBody");
	lua_.new_usertype<CBackgroundColor>("CBackgroundColor");*/

	sol::usertype<EntityManager> entity_manager = lua_.new_usertype<EntityManager>("EntityManager", sol::no_constructor);
	entity_manager["AddEntity"] = &EntityManager::addEntity;
	entity_manager["GetEntities"] = &EntityManager::getEntities;
	entity_manager["RemoveEntity"] = &EntityManager::removeEntity;
	entity_manager["CloneEntity"] = &EntityManager::cloneEntity;
	entity_manager["Reset"] = &EntityManager::reset;

	lua_.set("EntityManager", std::ref(EntityManager::GetInstance()));
	lua_.set("AttachedEntity", attached_entity);
	safe_script_update_ = lua_["Update"];
	safe_script_start_ = lua_["Start"];
	safe_script_update_.set_default_handler(lua_["got_problems"]);
}

sol::state& LuaState::GetSolState()
{
	return lua_;
}

LuaState::~LuaState()
{
}

void LuaState::Update()
{
	if (current_frame == 0)
	{
		safe_script_start_();
	}
	else
	{
		auto result = safe_script_update_();
		if (!result.valid())
		{
			sol::error err = result;
			std::cout << "Error: " << err.what() << std::endl;
		}
	}
	current_frame++;
}
