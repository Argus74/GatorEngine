#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>
#include "Vec2.h"
class CTransform {
public:
	Vec2 position, scale, velocity;
	float angle;
	CTransform() : position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
	CTransform(const Vec2& pos, const Vec2& scl, float ang) 
		: position(pos), scale(scl), angle(ang) {}
}; 

class CName {
public:
	std::string name;
	CName() : name("Default") {}
	CName(const std::string& n) : name(n) {}
};

class CShape {
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


class Entity {
	size_t m_id;
	std::string m_tag;
	bool is_alive;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CRigidBody> cRigidBody;

	Entity(const std::string& tag, size_t id);
	Entity();
	~Entity();

	void destroy();
	const std::string& tag() const;
	bool isAlive();
	// Component Accessors

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
};

#endif // ENTITY_H

