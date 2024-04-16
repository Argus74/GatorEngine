#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) : id_(id), tag_(tag), is_alive_(true) {
	// Initialization of components, if required
}

Entity::Entity() : id_(0), tag_("Default"), is_alive_(true) {
	// Default initialization
}

Entity::~Entity() {
	// Cleanup, if required
}

Entity::Entity(const Entity& rhs) {
	clone(rhs);
}

Entity& Entity::operator=(const Entity& rhs) {
	clone(rhs);
	return *this;
}

Entity::Entity(Entity&& rhs) {
	clone(rhs);
}

Entity& Entity::operator=(Entity&& rhs) {
	clone(rhs);
	return *this;
}

void Entity::clone(const Entity& other) {
	id_ = other.id_ + 1; // Increment the ID of the new entity
	tag_ = other.tag_;
	is_alive_ = other.is_alive_;
	disabled_ = other.disabled_;

	// Iterate through each component of other and deep copy its non-null components
	forEachComponent([&](auto& component, size_t index) {
		auto otherComponent = other.getComponent(component);
		if (otherComponent) {
			addComponent(otherComponent);
		}
	});

	// If clone needs a rigid body, add it to the physics world
	if (hasComponent<CRigidBody>()) {
		GatorPhysics::GetInstance().createBody(this, true);
	}
}

void Entity::destroy() {
	is_alive_ = false;
}

size_t Entity::id() const
{
	return id_;
}

bool Entity::isAlive() {
	return is_alive_;
}

bool Entity::isDisabled() {
	return disabled_;
}

void Entity::setDisabled(bool disable) {
	disabled_ = disable;
}

bool Entity::updateHealth(float dmg) { // Return true if there is a health component to take damage from
	if (this->hasComponent<CHealth>()) {
		auto healthComponent = this->getComponent<CHealth>();
		
		if (healthComponent->UpdateHealth(dmg))
			disabled_ = true;
		
		return true;
	}
	else {
		return false;
	}
}