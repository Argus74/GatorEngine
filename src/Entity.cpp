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

void Entity::destroy() {
	is_alive_ = false;
}

size_t Entity::id() const
{
	return id_;
}

const std::string& Entity::tag() const {
	return tag_;
}

bool Entity::isAlive() {
	return is_alive_;
}