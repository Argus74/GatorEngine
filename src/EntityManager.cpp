#include "EntityManager.h"
#include <algorithm>

// Factory function to get the singleton instance
EntityManager& EntityManager::GetInstance() {
	static EntityManager instance;
	return instance;
}

// Constructor
EntityManager::EntityManager() {}

// Add an entity with a given tag
std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto newEntity = std::make_shared<Entity>(tag, m_totalEntities++);
	m_toAdd.push_back(newEntity);
	return newEntity;
}

void EntityManager::cloneEntity(const std::shared_ptr<Entity>& entity)
{
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(*entity); // Call cpy ctr
	m_totalEntities++; // Increment total entities out here too
	m_toAdd.push_back(newEntity);
}

// Update function called every frame
void EntityManager::update()
{
	// Add new entities to the main vector and map
	for (auto& entity : m_toAdd) {
		m_entities.push_back(entity);
	}
	m_toAdd.clear();

	// Remove dead entities from the main vector
	m_entities.erase(
		std::remove_if(m_entities.begin(), m_entities.end(),
			[](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
		m_entities.end()
	);

}

// Get all entities
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
	return m_entities;
}

//Remove entity from our entity manager's list
void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
{
	for (int i  = 0; i < m_entities.size(); i++) {
		if (m_entities[i] == entity) {
			m_entities.erase(m_entities.begin() + i);
			break;
		}
	}
}

//Clear our entity list
void EntityManager::reset()
{
	m_entities.clear();
	m_toAdd.clear();

	m_totalEntities = 0;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntitiesRenderingList() {
	return entitiesRenderingList_;
}


void EntityManager::sortEntitiesForRendering() {
	
}