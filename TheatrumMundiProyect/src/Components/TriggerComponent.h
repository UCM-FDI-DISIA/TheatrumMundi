#pragma once
#include "MouseIntaractionComponent.h"

#include <SDL.h>

class TriggerComponent : public MouseIntaractionComponent
{
public:
	__CMPID_DECL__(ecs::cmp::TRIGGER_COMPONENT)

	enum EventType { CURSOR_ENTERED, CURSOR_LEFT, AREA_ENTERED, AREA_LEFT };

	TriggerComponent();

	bool mouseIsIn();

	const std::list<ecs::entity_t>& getOverlappingEntities() {
		return _currentOverlappingEntities;
	}

	// Gets the entity that triggered the current callback
	std::list<ecs::entity_t> const& triggerContextEntities() { 
		return _callbackContextEntities;
	}

	void update() override;

private:

	void updateTriggerState();

	std::list<ecs::entity_t> _callbackContextEntities; // The entity that triggered the current callback

protected:

	// Handles the interaction of the cursor
	void handleMouseInput() override; 
	
	// Handles the interaction with other Area2D owners
	void handleAreaOverlapping(); 

	bool areaIsHovered(std::pair<Sint32, Sint32> mousePos);
	
	// Updates de list of entities that overlap
	void updateOverlappingEntities(); 

	bool _mouseIsInside = false;

	std::list<ecs::entity_t> _currentOverlappingEntities;
	
	// Struct that gives information about all the entities that produced a trigger event
	struct {
		std::list<ecs::entity_t> exitedEntities;
		std::list<ecs::entity_t> unchangedEntities;
		std::list<ecs::entity_t> enteredEntities;
	}_triggerState; 
};