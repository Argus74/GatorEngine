#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>

class CTransform; // Forward declarations for the component classes
class CName;
class CShape;
class CBBox;

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