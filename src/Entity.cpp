#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) : m_id(id), m_tag(tag), m_alive(true) {
	// Initialization of components, if required
}

Entity::Entity() : m_id(0), m_tag("Default"), m_alive(true) {
	// Default initialization
}

Entity::~Entity() {
	// Cleanup, if required
}

void Entity::destroy() {
	m_alive = false;
}

const std::string& Entity::tag() const {
	return m_tag;
}

std::shared_ptr<CTransform> Entity::getTransform() const {
	return cTransform;
}

void Entity::setTransform(const std::shared_ptr<CTransform>& transform) {
	cTransform = transform;
}

std::shared_ptr<CName> Entity::getNameComponent() const {
	return cName;
}

void Entity::setNameComponent(const std::shared_ptr<CName>& name) {
	cName = name;
}

std::shared_ptr<CShape> Entity::getShape() const {
	return cShape;
}

void Entity::setShape(const std::shared_ptr<CShape>& shape) {
	cShape = shape;
}

std::shared_ptr<CBBox> Entity::getBBox() const {
	return cBBox;
}

void Entity::setBBox(const std::shared_ptr<CBBox>& bbox) {
	cBBox = bbox;
}