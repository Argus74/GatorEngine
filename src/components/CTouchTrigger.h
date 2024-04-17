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
	std::map<std::string, Action> tagMap;
	CTouchTrigger() {}
};

#endif // TOUCH_TRIGGER_H