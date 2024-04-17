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
	newEntity->addComponent<CName>();
	newEntity->addComponent<CInformation>();
	newEntity->addComponent<CTransform>();
	to_add_.push_back(newEntity);
	EntityManager::GetInstance().sortEntitiesForRendering();
	return newEntity;
}

void EntityManager::cloneEntity(const std::shared_ptr<Entity>& entity)
{
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(*entity); // Call cpy ctr
	total_entities_++; // Increment total entities out here too
	to_add_.push_back(newEntity);
}

// Update function called every frame
void EntityManager::update()
{
	// Add new entities to the main vector and map
	for (auto& entity : m_toAdd) {
		m_entities.push_back(entity);
		EntityManager::GetInstance().sortEntitiesForRendering(); //Sorting render list
	}
	to_add_.clear();

	// Remove dead entities from the main vector
	entities_.erase(
		std::remove_if(entities_.begin(), entities_.end(),
			[](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
		entities_.end()
	);

}

// Get all entities
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
	return entities_;
}

//Remove entity from our entity list, rendering list, and physics world
void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
{
	for (int i  = 0; i < m_entities.size(); i++) {
		if (m_entities[i] != entity) continue;

		if (entity->hasComponent<CRigidBody>()) {
			GatorPhysics::GetInstance().destroyBody(entity.get());
		}

			m_entities.erase(m_entities.begin() + i);
		EntityManager::GetInstance().sortEntitiesForRendering(); //Resorting our Render List
	}
}

void EntityManager::resetPositions() {
	for (auto entity : entities_) {
		if (entity->hasComponent<CTransform>()) {
			auto transform = entity->getComponent<CTransform>();
			transform->resetPosition();	
		}
	}
}

//Clear our entity list
void EntityManager::reset()
{
	entities_.clear();
	entity_map_.clear();
	to_add_.clear();
	total_entities_ = 0;
	m_entities.clear();
	m_toAdd.clear();

	m_totalEntities = 0;
}

//Return Entity Manager's RenderingList
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntitiesRenderingList() {
	return entitiesRenderingList_;
}

//Sorts entities based off layer and order in the explorer window
void EntityManager::sortEntitiesForRendering() {
	entitiesRenderingList_ = m_entities;
	std::cout << "Sorted" << std::endl;
	std::stable_sort(entitiesRenderingList_.begin(), entitiesRenderingList_.end(), [](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
		return a->getComponent<CInformation>()->layer < b->getComponent<CInformation>()->layer; // Primary sort by layer
	});

	/*  Renderlist Debug output
	for (const std::shared_ptr<Entity>& a : entitiesRenderingList_) {
		std::cout << "Entity Name: " << a->getComponent<CName>()->name << ", Layer: " << a->getComponent<CInformation>()->layer << "  ";
	}
	std::cout << std::endl; */
}