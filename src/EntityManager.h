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

	void removeEntity(std::shared_ptr<Entity> entity);

	void cloneEntity(const std::shared_ptr<Entity>& entity);

	void reset();

	//Sort Entities in rendering order
	void sortEntitiesForRendering();

	//Get Entities rendering order
	std::vector<std::shared_ptr<Entity>>& getEntitiesRenderingList();
	
	// Get list of rendering for UI rendering
	std::vector<std::shared_ptr<Entity>>& EntityManager::getUIRenderingList();

	void addEntityToUIList(std::shared_ptr<Entity> ent);

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

	// Total number of entities ever created, for unique IDs
	size_t m_totalEntities = 0;

	// Vector that stores the true rendering order of Entities  
	EntityVec entitiesRenderingList_;

	// Vector that stores entities with UI or Health Components
	EntityVec entitiesUIList_;
};

#endif // ENTITYMANAGER_H
