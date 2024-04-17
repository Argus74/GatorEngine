#include "ActionBus.h"

ActionBus& ActionBus::GetInstance() {
	static ActionBus instance;
	return instance;
}

void ActionBus::Dispatch(const std::shared_ptr<Entity>& entity, Action action) {
	entity_actions_map_[entity][action] = true;
}

bool ActionBus::Received(const std::shared_ptr<Entity>& entity, Action action) {
	return entity_actions_map_[entity][action];
}

void ActionBus::Clear() {
	for (auto& entity : entity_actions_map_) {
		for (int i = 0; i < NUM_ACTIONS; i++) {
			entity.second[i] = false;
		}
	}
}
