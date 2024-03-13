#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) : m_id(id), m_tag(tag), is_alive(true) {
	// Initialization of components, if required
}

Entity::Entity() : m_id(0), m_tag("Default"), is_alive(true) {
	// Default initialization
}

Entity::~Entity() {
	// Cleanup, if required
}

void Entity::destroy() {
	is_alive = false;
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

std::shared_ptr<CRigidBody> Entity::getRigidBody() const
{
	return cRigidBody;
}

void Entity::setRigidBody(const std::shared_ptr<CRigidBody>& rigidBody)
{
	cRigidBody = rigidBody;
}
std::shared_ptr<CUserInput> Entity::getUserInput() const {
	return cUserInput;
}

void Entity::setUserInput(const std::shared_ptr<CUserInput>& userInput) {
	cUserInput = userInput;
}

bool Entity::isAlive() {
	return is_alive;
}