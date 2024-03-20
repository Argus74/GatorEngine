#pragma once
#include "Component.h"
class CName : public Component {
public:
	std::string name;
	CName() : Component("Name"),  name("Default") {}
	CName(const std::string& n) : Component("Name"), name(n) {}
};