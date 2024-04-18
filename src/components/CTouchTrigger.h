/*
* File:  CTouchTrigger
*
* eeeeeeee
*/

#ifndef TOUCH_TRIGGER_H
#define TOUCH_TRIGGER_H

#include "../ActionBus.h"
#include "Component.h"

class CTouchTrigger : public Component {
public:
	DECLARE_COMPONENT_NAME("Touch Trigger");

	std::string tag_ = "";
	TriggerAction action_ = TriggerAction::None;
	Vec2 triggerSize_ = Vec2(50, 50);

	CTouchTrigger() {}
};

#endif // TOUCH_TRIGGER_H