#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "Component.h"


class CCollectable : public Component {
public:
	DECLARE_COMPONENT_NAME("Collectable");
	
	bool isHealth_ = false; // Determines if the collectable adds points or not
	float pointsToAdd = 25.0;
	bool disappearOnTouch_ = true;


	CCollectable() {}

};

#endif // COLLECTABLE_H