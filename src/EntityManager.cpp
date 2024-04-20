#include "EntityManager.h"
#include "editor/Editor.h"
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
	std::cout << "Adding entity with tag: " << tag << std::endl;
	auto newEntity = std::make_shared<Entity>(tag, total_entities_++);
	newEntity->addComponent<CName>();
	newEntity->addComponent<CInformation>();
	newEntity->addComponent<CTransform>();
	to_add_.push_back(newEntity);
	sortEntitiesForRendering();
	UpdateUIRenderingList();
	return newEntity;
}

std::shared_ptr<Entity> EntityManager::addEntityStart(const std::string& tag)
{
	std::cout << "Adding entity with tag: " << tag << std::endl;
	auto newEntity = std::make_shared<Entity>(tag, total_entities_++);
	newEntity->addComponent<CName>();
	newEntity->addComponent<CInformation>();
	newEntity->addComponent<CTransform>();
	entities_.push_back(newEntity);
	sortEntitiesForRendering();
	UpdateUIRenderingList();
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
	for (auto& entity : to_add_) {
		entities_.push_back(entity);
		entity->initialized = true;
		sortEntitiesForRendering(); //Sorting render list
		UpdateUIRenderingList();
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
	for (int i  = 0; i < entities_.size(); i++) {
		if (entities_[i] != entity) continue;

		if (entity->hasComponent<CRigidBody>()) {
			GatorPhysics::GetInstance().destroyBody(entity.get());
		}

		entities_.erase(entities_.begin() + i);
		sortEntitiesForRendering(); //Resorting our Render List
		UpdateUIRenderingList();
	}
}

void EntityManager::resetPositions() {
	for (auto entity : entities_) {
		if (entity->hasComponent<CTransform>()) {
			auto transform = entity->getComponent<CTransform>();
			transform->resetPosition();	
		}
		if (entity->hasComponent<CHealth>()) { 
			auto health = entity->getComponent<CHealth>();
			if (health->reset()) // If the entity was killed then we Enable it 
				entity->setDisabled(false);
		}
		if (entity->hasComponent<CCollectable>()) {
			auto collectable = entity->getComponent<CCollectable>();
			if (collectable->reset())
				entity->setDisabled(false);
		}
	}
}
//Clear our entity list
void EntityManager::reset()
{
	entities_.clear();
	to_add_.clear();

	total_entities_ = 0;
}

//Return Entity Manager's RenderingList
std::vector<std::shared_ptr<Entity>>& EntityManager::getEntitiesRenderingList() {
	return entities_rendering_list_;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getUIRenderingList() {
	return entities_UI_list_;
}

//Sorts entities based off layer and order in the explorer window
void EntityManager::sortEntitiesForRendering() {
	entities_rendering_list_ = entities_;
	//std::cout << "Sorted" << std::endl;
	std::stable_sort(entities_rendering_list_.begin(), entities_rendering_list_.end(), [](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
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
	for (auto& entity : entities_) {
		if (entity->hasComponent<CHealth>() || entity->hasComponent<CText>()) {
			newList.push_back(entity);
		}
	}

	entities_UI_list_ = newList;
}

std::shared_ptr<Entity> EntityManager::getEntityByName(const std::string& name) {
	for (const auto& entity : entities_) {
		if (entity->hasComponent<CName>()) {
			auto& nameComponent = entity->getComponent<CName>();
			if (nameComponent->name == name) {
				return entity;
			}
		}
	}
	return nullptr; // Return nullptr if no entity with the given name is found
}