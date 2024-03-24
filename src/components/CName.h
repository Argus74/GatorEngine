#pragma once
#include "Component.h"
class CName : public Component {
public:
	DECLARE_COMPONENT_NAME("Name");
	std::string name;
	CName() : name("Default") {}
	CName(const std::string& n) : name(n) {}
};