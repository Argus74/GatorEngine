/**
 * File: Scene.h
 *
 * This file contains the Scene class, which represents a collection of entities
 * that make up a particular game scene. It includes functionality for serializing a scene
 * into a structured format and deserializing it back, effectively allowing for scene state
 * saving and loading.
 */

#pragma once
#include "Serializable.h"
#include "../EntityManager.h"
#include <vector>
#include <iostream>

class Scene : public Serializable {
public:
    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("assetManager");
        AssetManager::GetInstance().serialize(writer);
        writer.Key("entities");
        writer.StartObject();
        for (auto entity : EntityManager::GetInstance().getEntities()) {
            entity->serialize(writer);
        }
        writer.EndObject();
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        // AssetManager::GetInstance().deserialize(value["assetManager"]);
        for (auto it = value["entities"].MemberBegin(); it != value["entities"].MemberEnd(); it++) {
            auto entity = EntityManager::GetInstance().addEntity(it->name.GetString());
            entity->deserialize(it->value);

            if (entity->hasComponent<CRigidBody>()) {
                GatorPhysics::GetInstance().createBody(entity.get(), entity->getComponent<CRigidBody>()->static_body);
            }
        }

        EntityManager::GetInstance().update();
    }

};