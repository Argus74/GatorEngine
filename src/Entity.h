#pragma once

#include "Components.h"

#include <tuple>
#include <string>

#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Components.h"
#include "components/CUserInput.h"
#include "GatorPhysics.h"

class CTransform : public Component {
	public:
	Vec2 position, scale, velocity;
	float angle;
	CTransform() : position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
	CTransform(Vec2 pos) : position(pos), scale(Vec2(1, 1)), angle(0) {}
	CTransform(const Vec2& pos, const Vec2& scl, float ang) 
		: position(pos), scale(scl), angle(ang) {}
}; 

class CName : public Component {
public:
	std::string name;
	CName() : name("Default") {}
	CName(const std::string& n) : name(n) {}
};

class CShape : public Component {
public:
	std::string type;
	sf::Color color;
	CShape() : type("Rectangle"), color(sf::Color::White) {}
	CShape(const std::string& t, const sf::Color& c) : type(t), color(c) {}
};

class CRigidBody : public Component {
	public:
		bool staticBody;
		b2Body* body;
		b2Fixture* fixture;
		CRigidBody() : staticBody(true) {}
		CRigidBody(bool flag, b2Body* b, b2Fixture* f) : staticBody(flag), body(b), fixture(f) {}
};


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


