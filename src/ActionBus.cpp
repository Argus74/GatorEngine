#include "ActionBus.h"

ActionBus& ActionBus::GetInstance() {
	static ActionBus instance;
	return instance;
}

void ActionBus::Dispatch(std::shared_ptr<Entity> entity, Action action) {
	entityActionsMap[entity][action] = true;
}

bool ActionBus::Received(std::shared_ptr<Entity> entity, Action action) {
	return entityActionsMap[entity][action];
}

void ActionBus::Clear() {
	for (auto& entity : entityActionsMap) {
		for (int i = 0; i < NUM_ACTIONS; i++) {
			entity.second[i] = false;
		}
	}
}
