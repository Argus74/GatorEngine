#pragma once

#include "components/includes.h"

#include <tuple>
#include <string>

#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "GatorPhysics.h"

typedef std::tuple< //ass we add more components, we add them here
	std::shared_ptr<CName>,
	std::shared_ptr<CInformation>,
	std::shared_ptr<CTransform>,
	std::shared_ptr<CShape>,
	std::shared_ptr<CUserInput>,
	std::shared_ptr<CAnimation>,
	std::shared_ptr<CSprite>,
	std::shared_ptr<CRigidBody>,
	std::shared_ptr<CBackgroundColor>
> ComponentTuple;

class Entity {
	size_t id_;
	
	bool is_alive_;
	friend class EntityManager;
public:
	ComponentTuple m_components;

	Entity(const std::string& tag, size_t id);
	Entity();
	~Entity();
	Entity(const Entity&);
	Entity& operator=(const Entity&);
	Entity(Entity&&);
	Entity& operator=(Entity&&);
	void clone(const Entity&);
	void destroy();
	size_t id() const;

	bool isAlive();

	std::string tag_;
	int layer_ = 1; //

	// Component Accessors and Modifiers 

	// Check whether this component is initialized
	template <typename T>
	bool hasComponent() const {
		auto ptr = std::get<std::shared_ptr<T>>(m_components);
		return ptr != nullptr && ptr->has;
	}

	// Initialize a new component based on template type, with optional argument list for component constructor
	template <typename T, typename... TArgs>
	std::shared_ptr<T> addComponent(TArgs&&... mArgs) { // .. TArgs allows for any amount of components to be in the parameter
		auto component = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
		std::get<std::shared_ptr<T>>(m_components) = component;
		component->has = true; 
		return component;
	}

	// Retrieve the component of the templated type (read-only version)

	template <typename T>
	void addComponent(const std::shared_ptr<T>& otherComponent) { // if this errors out, try removing the const
		// If uninitialized, initialize a new component using argument's type
		if (!otherComponent) {
			addComponent<T>();
			return;
		}
		// Otherwise, initialize using the argument itself (aka, make a deep copy of it)
		auto newComponent = std::make_shared<std::remove_reference_t<decltype(*otherComponent)>>(*otherComponent);
		newComponent->has = true;
		getComponent<T>() = newComponent;
	}

	// Retrieve the component of the templated type (read-only version)
	template <typename T>
	const std::shared_ptr<T> getComponent() const {
		return std::get<std::shared_ptr<T>>(m_components);
	}

	// Retrieve the component of the templated type (read/write version)
	template <typename T>
	std::shared_ptr<T>& getComponent() {
		return std::get<std::shared_ptr<T>>(m_components);
	}


	// Retrieve the component of the argument type (read-only version)
	template <typename T>
	const std::shared_ptr<T> getComponent(const std::shared_ptr<T>& otherComponent) const {
		return getComponent<T>();
	}

	// Retrieve the component of the argument type (read/write version)
	template <typename T>
	std::shared_ptr<T>& getComponent(const std::shared_ptr<T>& otherComponent) {
		return getComponent<T>();
	}

	// Remove the component of the templated type
	template <typename T> 
	void removeComponent() {
		getComponent<T>().reset(); // Resetting the shared pointer to nullptr
		// The old component will automatically be destroyed if no other shared_ptr instances are pointing to it
	}

	// Remove the component of the argument type
	template <typename T>
	void removeComponent(const std::shared_ptr<T>& newComponent) {
		removeComponent<T>();
	}

	// Helper function(s) to iterate through the m_components tuple and apply some lambda. 
	// See PropertyWindow for example usage.
	template<std::size_t Index = 0, typename Func>
	typename std::enable_if <Index<std::tuple_size<ComponentTuple>::value>::type
	forEachComponent(Func func) {
		func(std::get<Index>(m_components), Index);
		forEachComponent<Index + 1>(func);
	}
	template<std::size_t Index = 0, typename Func>
	typename std::enable_if<Index == std::tuple_size<ComponentTuple>::value>::type
	forEachComponent(Func) {
		// End of recursion: do nothing
	}
};


