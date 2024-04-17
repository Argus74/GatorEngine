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
};