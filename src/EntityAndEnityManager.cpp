// Entity.h
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
	
	// Accessor and mutator functions for the CTransform component
	std::shared_ptr<CTransform> getTransform() const;
	void setTransform(const std::shared_ptr<CTransform>& transform);

	// Accessor and mutator functions for the CName component
	std::shared_ptr<CName> getNameComponent() const;
	void setNameComponent(const std::shared_ptr<CName>& name);

	// Accessor and mutator functions for the CShape component
	std::shared_ptr<CShape> getShape() const;
	void setShape(const std::shared_ptr<CShape>& shape);

	// Accessor and mutator functions for the CBBox component
	std::shared_ptr<CBBox> getBBox() const;
	void setBBox(const std::shared_ptr<CBBox>& bbox);
};

#endif // ENTITY_H

//=============================================================================

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

//=============================================================================
// NOTE: I will separate this into a header and cpp file
// Using 'using' instead of typedef for type aliasing
using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
    EntityVec m_entities;
	
	// To prevent entity validation, holds new entity to add in next frame
	EntityVec m_toAdd; 
    EntityMap m_entityMap;
	
	// Total number of entities ever created. Prevents duplicating unique ids.
    size_t m_totalEntities = 0; 

public:
    EntityManager();
	
	// Called in every frame to add contents in m_toAdd vector;
	void update();
    
	// Gives a tag to the entity and returns shared pointer to entity created.
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	
	//Returns a vector of all entities.
    EntityVec& getEntities();
	
	// Returns a vector of entities of a given type.
    EntityVec& getEntities(const std::string& tag);
};

// Function to add entities entity manager
std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    // Create a new Entity object
	// Unique id given based on increment of total entities
    auto newEntity = std::make_shared<Entity>(tag, m_totalEntities++);

    // Store it in the toAdd vector, so entity can be added in the next frame.
    m_toAdd.push_back(newEntity);

    // Store it in the map of tag->entityvector
    m_entityMap[tag].push_back(newEntity);

    // Return the shared pointer pointing to that entity
    return newEntity;
}

// Function called at the beginning of each frame to add entities
void EntityManager::update()
{
    // Add new entities to the main vector and map
    for (auto& entity : m_toAdd) {
        m_entities.push_back(entity);
        m_entityMap[entity->getTag()].push_back(entity);
    }
    m_toAdd.clear();

    // Remove dead entities from the main vector
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(), 
                       [](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
        m_entities.end()
    );

    // Remove dead entities from the map
    for (auto& pair : m_entityMap) {
        pair.second.erase(
            std::remove_if(pair.second.begin(), pair.second.end(), 
                           [](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
            pair.second.end()
        );
    }

    // Additional logic to remove empty vectors from the map, if necessary.
	// Used to ensure iterator remains valid through the loop.
    for (auto it = m_entityMap.begin(); it != m_entityMap.end(); /* no increment here */) {
        if (it->second.empty()) {
            it = m_entityMap.erase(it); // erase returns the next iterator
        } else {
            ++it; // only increment if we didn't erase
        }
    }
}

//=============================================================================
#include "EntityManager.h" 
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    ImGui::SFML::Init(window);

    // Instantiates EntityManager
    EntityManager entityManager;

    // Example of adding an entity to the manager with a specific tag
    auto myEntity = entityManager.addEntity("Player");

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // Update EntityManager (and all managed entities)
        entityManager.update();

        // Use ImGui to interact with game objects
        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text."); // Display some text

        // Interact with your entities through ImGui 
        // ...

        ImGui::End();

        window.clear();

        // Draw entities
        // Exampl for an entity that has a method to return their shape for SFML:
        // for (auto& entity : entityManager.getEntities()) {
        //     if (auto shapeComponent = entity->getShape()) {
        //         window.draw(*shapeComponent->getSFMLShape());
        //     }
        // }

        window.draw(shape); // Original SFML shape
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}



