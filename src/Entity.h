#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>
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


typedef std::tuple< //ass we add more components, we add them here
	CTransform,
	CName,
	CShape,
	CAnimation
> ComponentTuple;


class Entity {
	size_t m_id;
	std::string m_tag;
	bool is_alive;
	ComponentTuple m_components;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CUserInput> cUserInput;

	Entity(const std::string& tag, size_t id);
	Entity();
	~Entity();

	void destroy();
	const std::string& tag() const;
	bool isAlive();

	// Component Accessors and Modifiers 
	template <typename T>
	bool hasComponent() const {
		return getComponent<T>().has;
	}

	template <typename T, typename... TArgs> 
	T& addComponent(TArgs&&... mArgs) { // .. TArgs allows for any amount of components to be in the parameter
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(margs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& getComponent() const {   //When using getComponent make sure to do auto& transform1 = entity->get<cTransform>() Rather than auto transform1 = entity->get<cTransform>()
		return std::get<T>(m_components);
	}

	template <typename T> 
	void removeComponent() {
		getComponent<T>() = T(); //We delete the component by just setting the CComponent as the defualt constructor, we should
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

	// Accessor and mutator for the CUserInput component
	std::shared_ptr<CUserInput> getUserInput() const;
	void setUserInput(const std::shared_ptr<CUserInput>& userInput);

};

#endif // ENTITY_H

