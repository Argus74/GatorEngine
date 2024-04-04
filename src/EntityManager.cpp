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
	auto newEntity = std::make_shared<Entity>(tag, total_entities_++);
	to_add_.push_back(newEntity);
	return newEntity;
}

// Update function called every frame
void EntityManager::update()
{
	// Add new entities to the main vector and map
	for (auto& entity : to_add_) {
		entities_.push_back(entity);
		entity_map_[entity->tag()].push_back(entity);
	}
	to_add_.clear();

	// Remove dead entities from the main vector
	entities_.erase(
		std::remove_if(entities_.begin(), entities_.end(),
			[](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
		entities_.end()
	);

	// Remove dead entities from the map
	for (auto& pair : entity_map_) {
		pair.second.erase(
			std::remove_if(pair.second.begin(), pair.second.end(),
				[](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
			pair.second.end()
		);
	}

	// Additional logic to remove empty vectors from the map, if necessary.
	// Used to ensure iterator remains valid through the loop.
	for (auto it = entity_map_.begin(); it != entity_map_.end();) {
		if (it->second.empty()) {
			it = entity_map_.erase(it); // erase returns the next iterator
		}
		else {
			++it; // only increment if we didn't erase
		}
	}
}

// Get all entities
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
	return entities_;
}

// Get entities with a specific tag
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities(const std::string& tag)
{
	return entity_map_[tag];
}

void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
{
	for (int i  = 0; i < entities_.size(); i++) {
		if (entities_[i] == entity) {
			entities_.erase(entities_.begin() + i);
			break;
		}
	}
	for (auto& pair : entity_map_) {
		for (int i = 0; i < pair.second.size(); i++) {
			if (pair.second[i] == entity) {
				pair.second.erase(pair.second.begin() + i);
				break;
			}
		}
	}
}

void EntityManager::reset()
{
	entities_.clear();
	entity_map_.clear();
	to_add_.clear();
	total_entities_ = 0;
}
