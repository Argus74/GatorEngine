#pragma once
#include "Serializable.h"
#include "../EntityManager.h"
#include <vector>
#include <iostream>

class Scene : public Serializable {
public:
    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("entities");
        writer.StartArray();
        for (auto entity : EntityManager::GetInstance().getEntities()) {
            entity.get()->serialize(writer);
        }
        writer.EndArray();
        writer.Key("assets");
        writer.StartArray();
        writer.EndArray();
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        for (auto it = value["entities"].GetArray().Begin(); it != value["entities"].GetArray().End(); it++) {
            auto entity = EntityManager::GetInstance().addEntity(it->GetObject()["tag"].GetString());
        }
    }

};