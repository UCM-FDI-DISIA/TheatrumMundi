#include "BehaviorStateComponent.h"

#include <assert.h>

BehaviorStateComponent::BehaviorStateComponent()
	: _currentState(0)
{
}

BehaviorStateComponent::~BehaviorStateComponent()
{
}

void BehaviorStateComponent::defBehavior(BEHAVIOR_STATE newState, std::function<void()> stateUpdate)
{
	if (_stateBehaviors.find(newState) == _stateBehaviors.end())
	{
		_stateBehaviors.insert({ newState, stateUpdate });
		
		return;
	}

	_stateBehaviors.at(newState) = stateUpdate; // Changes the existing state behavior
}

void BehaviorStateComponent::setState(BEHAVIOR_STATE state)
{
	//assert(_stateBehaviors.find(state) != _stateBehaviors.end());

	_currentState = state;
}

void BehaviorStateComponent::update()
{
	if (_stateBehaviors.empty()) return;

	if (_stateBehaviors.find(_currentState) == _stateBehaviors.end()) return;

	auto& stateBehavior = _stateBehaviors.at(_currentState);

	stateBehavior();
}

