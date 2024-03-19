#pragma once

#include "components/includes.h"

#include <tuple>
#include <string>

#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "GatorPhysics.h"




typedef std::tuple< //ass we add more components, we add them here
	std::shared_ptr<CTransform>,
	std::shared_ptr<CName>,
	std::shared_ptr<CShape>,
	std::shared_ptr<CUserInput>,
	std::shared_ptr<CAnimation>,
	std::shared_ptr<CSprite>,
	std::shared_ptr<CRigidBody>

> ComponentTuple;



class Entity {
	size_t id_;
	std::string tag_;
	bool is_alive_;
	friend class EntityManager;
public:
	ComponentTuple m_components;

	Entity(const std::string& tag, size_t id);
	Entity();
	~Entity();
	void destroy();
	size_t id() const;

	const std::string& tag() const;
	bool isAlive();
	// Component Accessors and Modifiers 
	template <typename T>
	bool hasComponent() const {
		auto ptr = std::get<std::shared_ptr<T>>(m_components);
		return ptr != nullptr && ptr->has;
	}

	template <typename T, typename... TArgs>
	std::shared_ptr<T> addComponent(TArgs&&... mArgs) { // .. TArgs allows for any amount of components to be in the parameter
		auto component = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
		std::get<std::shared_ptr<T>>(m_components) = component;
		component->has = true; 
		return component;
	}

	template <typename T>
	std::shared_ptr<T> getComponent() const {
		return std::get<std::shared_ptr<T>>(m_components);
	}

	template <typename T> 
	void removeComponent() {
		getComponenent<T>() = T(); // Resetting the shared pointer to nullptr
		// The old component will automatically be destroyed if no other shared_ptr instances are pointing to it
	}
};


