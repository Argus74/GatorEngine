#pragma once
#include "Component.h"
class CName : public Component {
public:
	std::string name;
	CName() : name("Default") {}
	CName(const std::string& n) : name(n) {}
};