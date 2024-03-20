#pragma once

#include "../AssetManager.h"
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

class Component
{
public:
	const char* componentName;
	bool has;
	Component(const char* n) : componentName(n), has(false) {} // This ctr forces the componentname to be set
};

