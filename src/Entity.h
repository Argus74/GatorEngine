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
	std::shared_ptr<CBackgroundColor>,
	std::shared_ptr<CHealth>,
	std::shared_ptr<CText>,
	std::shared_ptr<CTouchTrigger>,
	std::shared_ptr<CCharacter>,
	std::shared_ptr<CScript>

> ComponentTuple;

class Entity {
	size_t id_;

	bool is_alive_;
	friend class EntityManager;
	bool disabled_ = false;
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
	bool isDisabled();
	void setDisabled(bool disable);
	bool updateHealth(float dmg);

	// Helper to get the entity's sf::Rect (a "bounding box"), based on the components it has
	sf::FloatRect& GetRect(float margin = 0.0f);

	std::string tag_;
	int layer_ = 1; //Don't need this here to be honest

	// Component Accessors and Modifiers 

	// Check whether this component is initialized
	template <typename T>
	bool hasComponent() const {
		auto ptr = std::get<std::shared_ptr<T>>(m_components);
		return ptr != nullptr && ptr->has;
	}

	//Used for compatability with lua
	bool hasComponent(std::string componentName) {
		if (componentName == "CName")
		{
			return hasComponent<CName>();
		}
		else if (componentName == "CInformation")
		{
			return hasComponent<CInformation>();
		}
		else if (componentName == "CTransform")
		{
			return hasComponent<CTransform>();
		}
		else if (componentName == "CShape")
		{
			return hasComponent<CShape>();
		}
		else if (componentName == "CUserInput")
		{
			return hasComponent<CUserInput>();
		}
		else if (componentName == "CAnimation")
		{
			return hasComponent<CAnimation>();
		}
		else if (componentName == "CSprite")
		{
			return hasComponent<CSprite>();
		}
		else if (componentName == "CRigidBody")
		{
			return hasComponent<CRigidBody>();
		}
		else if (componentName == "CBackgroundColor")
		{
			return hasComponent<CBackgroundColor>();
		}
		else if (componentName == "CCharacter")
		{
			return hasComponent<CCharacter>();
		}
		else if (componentName == "CHealth")
		{
			return hasComponent<CHealth>();
		}
		else if (componentName == "CText")
		{
			return hasComponent<CText>();
		}
		else
		{
			return false;
		}
	}

	// Initialize a new component based on template type, with optional argument list for component constructor
	template <typename T, typename... TArgs>
	std::shared_ptr<T> addComponent(TArgs&&... mArgs) { // .. TArgs allows for any amount of components to be in the parameter
		auto component = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
		std::get<std::shared_ptr<T>>(m_components) = component;
		component->has = true;
		return component;
	}
	//Used for compatability with lua
	void addComponent(std::string componentName) {
		if (componentName == "CName")
		{
			addComponent<CName>();
		}
		else if (componentName == "CInformation")
		{
			addComponent<CInformation>();
		}
		else if (componentName == "CTransform")
		{
			addComponent<CTransform>();
		}
		else if (componentName == "CShape")
		{
			addComponent<CShape>();
		}
		else if (componentName == "CUserInput")
		{
			addComponent<CUserInput>();
		}
		else if (componentName == "CAnimation")
		{
			addComponent<CAnimation>();
		}
		else if (componentName == "CSprite")
		{
			addComponent<CSprite>();
		}
		else if (componentName == "CRigidBody")
		{
			addComponent<CRigidBody>();
		}
		else if (componentName == "CBackgroundColor")
		{
			addComponent<CBackgroundColor>();
		}
		else if (componentName == "CCharacter")
		{
			addComponent<CCharacter>();
		}
		else if (componentName == "CHealth")
		{
			addComponent<CHealth>();
		}
		else if (componentName == "CText")
		{
			addComponent<CText>();
		}
		else
		{
			return;
		}
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
	typename std::enable_if < Index<std::tuple_size<ComponentTuple>::value>::type
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


