#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <cassert>
#include <vector>

class Transform;

class ScrollComponent : public ecs::Component {
private:
	Vector2D _dir;
	float _timeScroll;
	float _initialTimeScroll; //Activates if this time > 0
	bool _isRight;
	bool _isScrolling = false;
public:

	__CMPID_DECL__(ecs::cmp::SCROLL_COMPONENT)

	ScrollComponent(Vector2D dir, float time);
	~ScrollComponent();
	//void initComponent() override;
	void Scroll();
	void update() override;
	bool isScrolling();
	void addElementToScroll(Transform* _objectT);
private:
	//Transform *_myTransform;
	std::vector<Transform*> _objectsTransform;
};