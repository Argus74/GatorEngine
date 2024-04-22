#include "Entity.h"

#include "GatorPhysics.h"

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
    id_ = other.id_;  // ID should be changed after, by the EM, to reflect the new entity
    tag = other.tag;
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

size_t Entity::id() const {
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

void Entity::updateHealth(
    float dmg) {  // Return true if there is a health component to take damage from
    auto healthComponent = this->getComponent<CHealth>();

    if (healthComponent->UpdateHealth(dmg)) {  // True if the player "died"
        if (healthComponent->respawn_character) {
            this->getComponent<CTransform>()->position = healthComponent->respawn_position;
        } else {  // We disable our character if the Dev doesn't want to respawn the entity
            disabled_ = true;
        }
    }
}

sf::FloatRect& Entity::GetRect(float margin) {
    auto& transform = *(getComponent<CTransform>());
    sf::FloatRect rect;

    if (hasComponent<CSprite>()) {  // Set selection box size, depending on the entity's current sprite
        auto& sprite = getComponent<CSprite>()->sprite;
        // Set selection box size, depending on the entity's current sprite
        rect.width = sprite.getGlobalBounds().width + margin;
        rect.height = sprite.getGlobalBounds().height + margin;
    } else if (hasComponent<CAnimation>()) {
        auto& sprite = getComponent<CAnimation>()->animation.sprite;
        sprite.setScale(transform.scale.x,
                        transform.scale.y);  // Hacky fix to get sprite with correct scale
        rect.width = sprite.getGlobalBounds().width + margin;
        rect.height = sprite.getGlobalBounds().height + margin;
    }  else if (hasComponent<CText>()) {
		auto& text = getComponent<CText>()->text;
		sf::FloatRect bounds = text.getGlobalBounds();
		rect.width = bounds.width;
		rect.height = bounds.height;
	} 
    else {  // Changed to 50 rather than to base off transform scale, as transform scale caused the selection box to be too small
        rect.width = 50 * transform.scale.x;
        rect.height = 50 * transform.scale.y;
    }

    // TODO: Use collision boxes
    // TODO: Take largest of all components

    // Get top-left corner of the entity
    rect.left = transform.position.y - (rect.height / 2);
    rect.top = transform.position.x - (rect.width / 2);

    return rect;
}