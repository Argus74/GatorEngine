#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "GatorPhysics.h"

class EntityManager
{
public:
	// Factory function to get the singleton instance
	static EntityManager& GetInstance();

	// Update function called every frame
	void update();

	// Add an entity with a given tag
	std::shared_ptr<Entity> addEntity(const std::string& tag);

	// Get all entities
	std::vector<std::shared_ptr<Entity>>& getEntities();

	// Get entities with a specific tag
	std::vector<std::shared_ptr<Entity>>& getEntities(const std::string& tag);

	void removeEntity(std::shared_ptr<Entity> entity);

	void reset();

private:
	// Constructor
	EntityManager();

	// Type aliases for cleaner code
	using EntityVec = std::vector<std::shared_ptr<Entity>>;
	using EntityMap = std::unordered_map<std::string, EntityVec>;

	// Vector to store all entities
	EntityVec m_entities;

	// Vector to store entities to be added in the next frame
	EntityVec m_toAdd;

	// Map to store entities based on their tag
	EntityMap m_entityMap;

	// Total number of entities ever created, for unique IDs
	size_t m_totalEntities = 0;
};

#endif // ENTITYMANAGER_H
