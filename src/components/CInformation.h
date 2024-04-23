/*
* File:  CInformation.h
*
* This component is used to keep track of critical properties such as the Entities layer and tag.
*/

#pragma once

#include "Component.h"

class CInformation : public Component {
 public:
    DECLARE_COMPONENT_NAME("Information");
    int layer = 1;
    std::string tag = "Default";
    bool selectable = true;
    CInformation() {}

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("layer");
        writer.Int(layer);
        writer.Key("tag");
        writer.String(tag.c_str());
        writer.Key("selectable");
        writer.Bool(selectable);
        writer.EndObject();
    }
    void deserialize(const rapidjson::Value& value) override {
        layer = value["layer"].GetInt();
        tag = value["tag"].GetString();
        selectable = value["selectable"].GetBool();
    }
};