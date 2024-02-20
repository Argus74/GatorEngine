#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

// TODO: Implement components 
// These definitions are just placeholders so we can demo :)
class CTransform {
public:
	sf::Vector2f position, scale, velocity;
	float angle;
	CTransform() : position(sf::Vector2f(0, 0)), scale(sf::Vector2f(1, 1)), angle(0) {}
	CTransform(const sf::Vector2f& pos, const sf::Vector2f& scl, float ang) 
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

class CBBox {
public:
	sf::Vector2f size;
	bool isStatic;
	CBBox() : size(sf::Vector2f(0, 0)) {}
	CBBox(const sf::Vector2f& s) : size(s) {}
};

class Entity {
	size_t m_id;
	std::string m_tag;
	bool m_alive;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CBBox> cBBox;

	Entity(const std::string& tag, size_t id);
	Entity();
	~Entity();

	void destroy();
	const std::string& tag() const;

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

	// Accessor and mutator for the CBBox component
	std::shared_ptr<CBBox> getBBox() const;
	void setBBox(const std::shared_ptr<CBBox>& bbox);

	bool isAlive();
};

#endif // ENTITY_H