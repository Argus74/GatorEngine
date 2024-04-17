#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) : id_(id), tag(tag), is_alive_(true) {
	// Initialization of components, if required
}

Entity::Entity() : id_(0), tag("Default"), is_alive_(true) {
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
	tag = other.tag;
	is_alive_ = other.is_alive_;

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

sf::FloatRect& Entity::GetRect(float margin) {
	auto& transform = *(getComponent<CTransform>());
	sf::FloatRect rect;

	// Set selection box size, depending on the entity's current sprite
	if (hasComponent<CSprite>()) {
		auto& sprite = getComponent<CSprite>()->sprite;
		rect.width = sprite.getGlobalBounds().width + margin;
		rect.height = sprite.getGlobalBounds().height + margin;
	}
	else if (hasComponent<CAnimation>()) {
		auto& sprite = getComponent<CAnimation>()->animation.sprite;
		sprite.setScale(transform.scale.x, transform.scale.y); // Hacky fix to get sprite with correct scale
		rect.width = sprite.getGlobalBounds().width + margin;
		rect.height = sprite.getGlobalBounds().height + margin;
	}
	else {
		// If no sprite or animation, I guess just use the transform's scale?
		rect.width = transform.scale.x;
		rect.height = transform.scale.y;
	}
	// TODO: Use collision boxes
	// TODO: Take largest of all components

	// Get top-left corner of the entity
	rect.left = transform.origin.y - (rect.height / 2);
	rect.top = transform.origin.x - (rect.width / 2);

	return rect;
}