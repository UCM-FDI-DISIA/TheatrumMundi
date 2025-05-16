#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "WriteTextComponent.h"
#include "TextInfo.h"
class Image;
class Transform;


class DialogueAnimComponent : public ecs::Component
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
	WriteTextComponent<TextInfo>* _mytext;
	float _movespeed = 1;
	float actmovespeed = 1;
	float maxSpeed = 20;
	int _alphaspeed = 20;
	int posCooldown = 0;
	int distantoToanim;
	Vector2D originalPos;
	
	AnimState myAnim;
public:
	__CMPID_DECL__(ecs::cmp::DIALOGUE_ANIM_COMPONENT)
	DialogueAnimComponent(int movespeed = 1, int alphaspeed = 20);
	~DialogueAnimComponent();
	void startDialogueAnim();
	void endDialogueAnim();
	void update() override;
	
};

