/*
* File:   CBackgroundColor.h
* 
* This component is used to determine the clear color of the program, which happens before
* any scene or editor rendering is done (see main.cpp and GameEngine::update()).
*/

#pragma once

#include "Component.h"

class CBackgroundColor : public Component {
 public:
    DECLARE_COMPONENT_NAME("Background Color");
    sf::Color color;
    CBackgroundColor() : color(sf::Color::Blue) {}

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("r");
        writer.Int(color.r);
        writer.Key("g");
        writer.Int(color.g);
        writer.Key("b");
        writer.Int(color.b);
        writer.Key("a");
        writer.Int(color.a);
        writer.EndObject();
    }
    void deserialize(const rapidjson::Value& value) override {
        color = sf::Color(value["r"].GetInt(), value["g"].GetInt(), value["b"].GetInt(),
                          value["a"].GetInt());
    }
};