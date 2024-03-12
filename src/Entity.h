#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "Vec2.h"
class CTransform {
public:

#include "Components.h"
#include "components/CUserInput.h"
#include "Vec2.h"

class CTransform : public Component {
	public:
	Vec2 position, scale, velocity;
	float angle;
	CTransform() : position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
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

class CRigidBody {
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
	std::shared_ptr<CSprite>
> ComponentTuple;



typedef std::tuple< //ass we add more components, we add them here
	std::shared_ptr<CTransform>,
	std::shared_ptr<CName>,
	std::shared_ptr<CShape>,
	std::shared_ptr<CUserInput>,
	std::shared_ptr<CAnimation>,
	std::shared_ptr<CSprite>
> ComponentTuple;


class Entity {
	size_t m_id;
	std::string m_tag;
	bool is_alive;
	ComponentTuple m_components;

public:
	std::shared_ptr<CTransform> cTransform; //For now Not commenting//
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CRigidBody> cRigidBody;
  std::shared_ptr<CUserInput> cUserInput;

	ComponentTuple m_components;

	Entity(const std::string& tag, size_t id);
	Entity();
	~Entity();

	void destroy();
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
		component = std::make_shared<T>(); // Replacing the component with a new default-constructed one
		// The old component will automatically be destroyed if no other shared_ptr instances are pointing to it
	}
		

	// Accessor and mutator for the CTransform component
	std::shared_ptr<CTransform> getTransform() const;
	void setTransform(const std::shared_ptr<CTransform>& transform);

	// Accessor and mutator for the CName component
	std::shared_ptr<CName> getNameComponent() const;
	void setNameComponent(const std::shared_ptr<CName>& name);

	// Accessor and mutator for the CShape component
	std::shared_ptr<CShape> getShape() const;
	void setShape(const std::shared_ptr<CShape>& shape);

	// Accessor and mutator for the CRigidBody component
	std::shared_ptr<CRigidBody> getRigidBody() const;
	void setRigidBody(const std::shared_ptr<CRigidBody>& rigidBody);
	// Accessor and mutator for the CUserInput component
	std::shared_ptr<CUserInput> getUserInput() const;
	void setUserInput(const std::shared_ptr<CUserInput>& userInput);
};

#endif // ENTITY_H

