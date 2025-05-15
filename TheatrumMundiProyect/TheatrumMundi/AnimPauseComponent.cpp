#include "AnimPauseComponent.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
AnimPauseComponent::AnimPauseComponent(int movespeed, int alphaspeed)
{
	_movespeed = movespeed*Game::Instance()->hscreenScale;
	maxSpeed *= Game::Instance()->hscreenScale;
	_alphaspeed = alphaspeed;
	myImg = nullptr;
	myTransform = nullptr;
	myAnim = STOP;
	distantoToanim = -500 * Game::Instance()->hscreenScale;
}

AnimPauseComponent::~AnimPauseComponent()
{
}

void AnimPauseComponent::startPauseAnim()
{
	myAnim = START;
	myTransform = getContext()->getMngr()->getComponent<Transform>(getContext());
	myImg = getContext()->getMngr()->getComponent<Image>(getContext());
	originalPos = myTransform->getPos();
	posCooldown = distantoToanim;
	actmovespeed = _movespeed;
	if (myImg != nullptr)myImg->setPosOffset(Vector2D(0, distantoToanim));
}

void AnimPauseComponent::endPauseAnim()
{
	myAnim = END;
	posCooldown = 0;
	actmovespeed = _movespeed;
}

void AnimPauseComponent::update()
{
	switch (myAnim)
	{
	case AnimPauseComponent::STOP:
		break;
	case AnimPauseComponent::START:

		if (myImg != nullptr)myImg->addPosOffset(0, actmovespeed);
		posCooldown += actmovespeed;
		if (actmovespeed < maxSpeed) actmovespeed += 1;

		if (posCooldown > 0) {
			myTransform->setPosPure(originalPos);
			if (myImg != nullptr)myImg->setPosOffset(0, 0);
			myAnim = STOP;
		}
		break;
	case AnimPauseComponent::END:
		if (myImg != nullptr)myImg->addPosOffset(0, -actmovespeed);
		posCooldown -= actmovespeed;
		if (actmovespeed < maxSpeed) actmovespeed += 1;
		if (posCooldown < (distantoToanim)) {
		
		
			//myTransform->setPosPure(originalPos);
			myAnim = STOP;
			if (myImg != nullptr)myImg->setPosOffset(0, 0);
			getContext()->getMngr()->setActive(getContext(), false);
		}

		break;
	default:
		break;
	}
}