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

// Update function called every frame
void EntityManager::update()
{
	// Add new entities to the main vector and map
	for (auto& entity : m_toAdd) {
		m_entities.push_back(entity);
		m_entityMap[entity->tag()].push_back(entity);
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
	for (auto it = m_entityMap.begin(); it != m_entityMap.end();) {
		if (it->second.empty()) {
			it = m_entityMap.erase(it); // erase returns the next iterator
		}
		else {
			++it; // only increment if we didn't erase
		}
	}
}

// Get all entities
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
	return m_entities;
}

// Get entities with a specific tag
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}
