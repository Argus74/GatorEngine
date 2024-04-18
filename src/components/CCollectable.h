#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "Component.h"
#include "../Entity.h"

class CCollectable : public Component {
public:
	DECLARE_COMPONENT_NAME("Collectable");

	static enum CollectableText { // Using this to overload our property window functions as they are overloaded by type
		None
	};
	
	CollectableText temp = CollectableText::None;
	bool isHealth_ = false; // Determines if the collectable adds points or not
	float pointsToAdd = 25.0;
	bool disappearOnTouch_ = true;

	std::shared_ptr<Entity> collectableTextEntity;

	CCollectable() {}
};

#endif // COLLECTABLE_H