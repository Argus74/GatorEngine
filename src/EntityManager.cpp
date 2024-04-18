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
	newEntity->addComponent<CName>();
	newEntity->addComponent<CInformation>();
	newEntity->addComponent<CTransform>();
	m_toAdd.push_back(newEntity);
	sortEntitiesForRendering();
	UpdateUIRenderingList();
	return newEntity;
}

void EntityManager::cloneEntity(const std::shared_ptr<Entity>& entity)
{
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(*entity); // Call cpy ctr
	newEntity->id_ = m_totalEntities++;
	m_toAdd.push_back(newEntity);
}

// Update function called every frame
void EntityManager::update()
{
	// Add new entities to the main vector and map
	for (auto& entity : m_toAdd) {
		m_entities.push_back(entity);
		sortEntitiesForRendering(); //Sorting render list
		UpdateUIRenderingList();
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

//Remove entity from our entity list, rendering list, and physics world
void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
{
	for (int i  = 0; i < m_entities.size(); i++) {
		if (m_entities[i] != entity) continue;

		if (entity->hasComponent<CRigidBody>()) {
			GatorPhysics::GetInstance().destroyBody(entity.get());
		}

		m_entities.erase(m_entities.begin() + i);
		sortEntitiesForRendering(); //Resorting our Render List
		UpdateUIRenderingList();
	}
}

//Clear our entity list
void EntityManager::reset()
{
	m_entities.clear();
	m_toAdd.clear();

	m_totalEntities = 0;
}

//Return Entity Manager's RenderingList
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntitiesRenderingList() {
	return entitiesRenderingList_;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getUIRenderingList() {
	return entitiesUIList_;
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


void EntityManager::UpdateUIRenderingList() {
	EntityVec newList;
	for (auto& entity : m_entities) {
		if (entity->hasComponent<CHealth>() || entity->hasComponent<CText>()) {
			newList.push_back(entity);
		}
	}

	entitiesUIList_ = newList;
}