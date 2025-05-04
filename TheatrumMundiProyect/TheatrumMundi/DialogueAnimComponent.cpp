#include "DialogueAnimComponent.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../Components/Transform.h"
#include "../Components/Image.h"
#include "../sdlutils/SDLUtils.h"
#include "../src/game/Game.h"
DialogueAnimComponent::DialogueAnimComponent(int movespeed , int alphaspeed)
{
	_movespeed = movespeed;
	_alphaspeed = alphaspeed;
	myAnim = STOP;

}

DialogueAnimComponent::~DialogueAnimComponent()
{
}

void DialogueAnimComponent::startDialogueAnim()
{
	myAnim = START;
	myTransform = getContext()->getMngr()->getComponent<Transform>(getContext());
	myImg = getContext()->getMngr()->getComponent<Image>(getContext());
	originalPos = myTransform->getPos();
	actmovespeed = _movespeed;
	myTransform->setPosPure(Vector2D(originalPos.getX(),originalPos.getY()+100* Game::Instance()->hscreenScale));

}

void DialogueAnimComponent::endDialogueAnim()
{
	myAnim = END;
}

void DialogueAnimComponent::update()
{
	switch (myAnim)
	{
	case DialogueAnimComponent::STOP:
		break;
	case DialogueAnimComponent::START:
		myTransform->addPos(0, -actmovespeed);
		if (actmovespeed < maxSpeed) actmovespeed+=1;
	
		if (myTransform->getPos().getY() < originalPos.getY()) {
			myTransform->setPosPure(originalPos);
			myAnim = STOP;
		}
		break;
	case DialogueAnimComponent::END:
		break;
	default:
		break;
	}
}
