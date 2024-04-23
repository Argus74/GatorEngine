/**
 * File: EntityManager.h
 *
 * The EntityManager class is responsible for managing all entities in the game.
 * It provides a centralized interface to update entities, add and remove entities,
 * clone entities, and manage the rendering order. It also offers methods to reset entity
 * positions and handle entity-related updates specific to the game's UI. The EntityManager
 * uses a singleton pattern to ensure that there is a single, global point of access to the
 * entity list.
 *
 * This class manages two primary lists of entities: one for all entities and one specifically
 * for UI components. Entities are added to a buffer first and then to the main list after updates
 * to prevent issues during iteration. Additionally, it provides utility functions for sorting
 * entities based on rendering order and finding entities by name.
 */

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "GatorPhysics.h"

class EntityManager {
 public:
    // Factory function to get the singleton instance
    static EntityManager& GetInstance();

    // Update function called every frame
    void update();

    // Add an entity with a given tag
    std::shared_ptr<Entity> addEntity(const std::string& tag);

    // Adding entities through add entity puts it in a buffer thats added after update. Need to add entity immediatley
    std::shared_ptr<Entity> addEntityStart(const std::string& tag);

    // Get all entities
    std::vector<std::shared_ptr<Entity>>& getEntities();

    void removeEntity(std::shared_ptr<Entity> entity);

    void cloneEntity(const std::shared_ptr<Entity>& entity);

    // Reset all entities with CTransform component to their origin after testing
    void resetPositions();

    void reset();

    //Sort Entities in rendering order
    void sortEntitiesForRendering();

    //Get Entities rendering order
    std::vector<std::shared_ptr<Entity>>& getEntitiesRenderingList();

    // Get list of rendering for UI rendering
    std::vector<std::shared_ptr<Entity>>& getUIRenderingList();

    // Updating our UI rendering list
    void UpdateUIRenderingList();

    std::shared_ptr<Entity> getEntityByName(const std::string& name);

 private:
    // Constructor
    EntityManager();

    // Type aliases for cleaner code
    using EntityVec = std::vector<std::shared_ptr<Entity>>;
    using EntityMap = std::unordered_map<std::string, EntityVec>;

    // Vector to store all entities
    EntityVec entities_;

    // Vector to store entities to be added in the next frame
    EntityVec to_add_;

    // Total number of entities ever created, for unique IDs
    size_t total_entities_ = 0;

    // Vector that stores the true rendering order of Entities
    EntityVec entities_rendering_list_;

    // Vector that stores entities with UI or Health Components
    EntityVec entities_UI_list_;
};

#endif  // ENTITYMANAGER_H
