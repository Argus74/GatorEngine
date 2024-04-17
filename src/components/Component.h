#pragma once
#include "../AssetManager.h"
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

// Helper macro to declare a Component class' "friendly" name
// Make sure you use this macro in the class definition (public section) of all components
#define DECLARE_COMPONENT_NAME(name) static constexpr const char* componentName = name;

class Component
{
public:
	bool has;
};

