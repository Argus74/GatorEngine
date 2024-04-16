#include "ActionBus.h"

ActionBus& ActionBus::GetInstance() {
	static ActionBus instance;
	return instance;
}

void ActionBus::Dispatch(const std::shared_ptr<Entity>& entity, Action action) {
	entityActionsMap[entity][action] = true;
}

bool ActionBus::Received(const std::shared_ptr<Entity>& entity, Action action) {
	return entityActionsMap[entity][action];
}

void ActionBus::Clear() {
	for (auto& entity : entityActionsMap) {
		for (int i = 0; i < NUM_ACTIONS; i++) {
			entity.second[i] = false;
		}
	}
}
