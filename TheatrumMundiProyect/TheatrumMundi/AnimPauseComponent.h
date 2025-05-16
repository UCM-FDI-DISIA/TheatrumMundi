#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../Components/Transform.h"
#include "../Components/Image.h"
class AnimPauseComponent:public ecs::Component
{
private:
	enum AnimState
	{
		STOP = 0,
		START,
		END
	};
	Image* myImg;
	Transform* myTransform;
	float _movespeed = 1;
	float actmovespeed = 1;
	int maxSpeed = 20;
	int _alphaspeed = 20;
	int posCooldown = 0;
	int distantoToanim;
	Vector2D originalPos;

	AnimState myAnim;
public:
	__CMPID_DECL__(ecs::cmp::ANIM_PAUSE_COMPONENT)
		AnimPauseComponent(int movespeed = 1, int alphaspeed = 20);
	~AnimPauseComponent();
	void startPauseAnim();
	void endPauseAnim();
	void update() override;

};

