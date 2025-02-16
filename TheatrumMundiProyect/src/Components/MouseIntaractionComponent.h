#pragma once
#include "../ecs/Component.h"

#include <unordered_map>
#include <vector>
#include <functional>

class MouseIntaractionComponent : public ecs::Component
{
public:

	using EVENT_TYPE = int; // Alias type that refers to the EventType enum, that will be extended
	using CALLBACK = std::function<void()>;

	enum EventType {}; // Type of mouse event

	MouseIntaractionComponent();
	virtual ~MouseIntaractionComponent();

	bool connect(EVENT_TYPE, CALLBACK);

protected:

	std::unordered_map<EVENT_TYPE, std::vector<CALLBACK>> _eventConnections;

	void update() override;

	virtual void handleMouseInput() = 0;
};