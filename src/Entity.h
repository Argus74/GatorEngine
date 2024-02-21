#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

class CTransform
{
public:
	sf::Vector2f position, scale, velocity;
	float angle;
	CTransform() : position(sf::Vector2f(0, 0)), scale(sf::Vector2f(1, 1)), angle(0) {}
	CTransform(const sf::Vector2f &pos, const sf::Vector2f &scl, float ang)
		: position(pos), scale(scl), angle(ang) {}
};

class CName
{
public:
	std::string name;
	CName() : name("Default") {}
	CName(const std::string &n) : name(n) {}
};

class CShape
{
public:
	std::string type;
	sf::Color color;
	CShape() : type("Rectangle"), color(sf::Color::White) {}
	CShape(const std::string &t, const sf::Color &c) : type(t), color(c) {}
};

class Entity
{
	size_t m_id;
	std::string m_tag;
	bool is_alive;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;

	Entity(const std::string &tag, size_t id);
	Entity();
	~Entity();

	void destroy();
	const std::string &tag() const;
	bool isAlive();
	// Component Accessors

	// Accessor and mutator for the CTransform component
	std::shared_ptr<CTransform> getTransform() const;
	void setTransform(const std::shared_ptr<CTransform> &transform);

	// Accessor and mutator for the CName component
	std::shared_ptr<CName> getNameComponent() const;
	void setNameComponent(const std::shared_ptr<CName> &name);

	// Accessor and mutator for the CShape component
	std::shared_ptr<CShape> getShape() const;
	void setShape(const std::shared_ptr<CShape> &shape);
};

#endif // ENTITY_H
