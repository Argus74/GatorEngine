/*
	File: ActionBus.h
	The ActionBus is a messaging system that allows entities to trigger actions (via Dispatch())
	and allows systems to check if an entity has triggered a certain action (via Received()). 
	This was created to decouple the user input system from the rest of the games' systems. 
	In future iterations, ActionBus can probably be modified to handle in-game events (think 
	'Touched' or 'PlayerMoved' kinds of events).
	For now, ActionBus is a global singleton and not thread-safe, since neither should be an issue
	at this stage of the project.

	GLUING NOTE: See below
	Example Usage:
	void sUserInput() {
	    for (each entity in the game with a cUserInput component)
	        if (entity was registered to do "MoveUp" on this key press)
	            ActionBus::GetInstance().Dispatch(entity, MoveUp);
	}
	void sMovement() {
		for (each entity in the game)
			if (ActionBus::GetInstance().Received(entity, MoveUp))
				move entity's cTransform in y- direction somehow
	}
*/

#ifndef ACTION_BUS_H
#define ACTION_BUS_H

#include <memory>
#include <map>

class Entity; // Forward declared to avoid a circular dependency

static const char* kActionNames[] = {
	"No Action",
	"Move Up",
	"Move Down",
	"Move Left",
	"Move Right",
};

static enum Action {
	NoAction,
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	NUM_ACTIONS
};

class ActionBus {
 public:
	static ActionBus& GetInstance();
	~ActionBus() = default;
	ActionBus(const ActionBus&) = delete;
	ActionBus& operator=(const ActionBus&) = delete;
	ActionBus(ActionBus&&) = delete;
	ActionBus& operator=(ActionBus&&) = delete;

	// Clear the map of all entities and their actions
	void Clear();

	// Inform the bus that the entity triggered an action
	void Dispatch(std::shared_ptr<Entity> entity, Action action);

	// Check to see if a certain entity has triggered a certain action
	bool Received(std::shared_ptr<Entity> entity, Action action);

 private:
	 ActionBus() = default;

	 // Map of entities and the actions they triggered
	 std::map<std::shared_ptr<Entity>, bool[NUM_ACTIONS]> entity_actions_map_;
};

#endif // ACTION_BUS_H