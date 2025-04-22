#pragma once
#include "../ecs/Component.h"

#include <unordered_map>
#include <functional>

class BehaviorStateComponent : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BEHAVIOR_STATE_COMPONENT)

	using BEHAVIOR_STATE = int;

	BehaviorStateComponent();
	virtual ~BehaviorStateComponent();

	// The update executed depends on the state
	void update() override;

	// Registers a new state to perform a different update function
	void addBehavior(BEHAVIOR_STATE newState, std::function<void()> stateUpdate);

	void setState(BEHAVIOR_STATE);

	// Gets the id (enum) of the current state. Usefull for checking the state in callbacks
	BEHAVIOR_STATE getState() { return _currentState; }

private:

	BEHAVIOR_STATE _currentState;

	std::unordered_map<BEHAVIOR_STATE, std::function<void()>> _stateBehaviors;
};