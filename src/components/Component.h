#pragma once
#include "../AssetManager.h"
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include "../util/Serializable.h"

// Helper macro to declare a Component class' "friendly" name
// Make sure you use this macro in the class definition (public section) of all components
#define DECLARE_COMPONENT_NAME(name) static constexpr const char* kComponentName = name;

class Component : public Serializable
{
public:
	bool has;

	virtual void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) = 0;
    virtual void deserialize(const rapidjson::Value& value) = 0;
};

