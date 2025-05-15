#include "InvAnimComponent.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
InvAnimComponent::InvAnimComponent(int movespeed, int alphaspeed)
{
	_movespeed = movespeed * Game::Instance()->wscreenScale;
	maxSpeed *= Game::Instance()->wscreenScale;
	_alphaspeed = alphaspeed;
	myImg = nullptr;
	myTransform = nullptr;
	myAnim = STOP;
	distantoToanim = +400 * Game::Instance()->wscreenScale;
}

InvAnimComponent::~InvAnimComponent()
{
}

void InvAnimComponent::startInvAnim()
{
	myAnim = START;
	myTransform = getContext()->getMngr()->getComponent<Transform>(getContext());
	myImg = getContext()->getMngr()->getComponent<Image>(getContext());
	originalPos = myTransform->getPos();
	posCooldown = distantoToanim;
	actmovespeed = _movespeed;
	if (myImg != nullptr)myImg->setPosOffset(Vector2D( distantoToanim,0));
}

void InvAnimComponent::endInvAnim()
{
	myTransform = getContext()->getMngr()->getComponent<Transform>(getContext());
	myImg = getContext()->getMngr()->getComponent<Image>(getContext());
	originalPos = myTransform->getPos();
	myAnim = END;
	posCooldown = 0;
	actmovespeed = _movespeed;
}

void InvAnimComponent::update()
{
	switch (myAnim)
	{
	case STOP:
		break;
	case START:

		if (myImg != nullptr)myImg->addPosOffset(-actmovespeed,0);
		posCooldown -= actmovespeed;
		if (actmovespeed < maxSpeed) actmovespeed += 1;

		if (posCooldown < 0) {
			myTransform->setPosPure(originalPos);
			if (myImg != nullptr)myImg->setPosOffset(0, 0);
			myAnim = STOP;
		}
		break;
	case END:
		if (myImg != nullptr)myImg->addPosOffset(+actmovespeed,0);
		posCooldown += actmovespeed;
		if (actmovespeed < maxSpeed) actmovespeed += 1;

		if (posCooldown > (distantoToanim)) {
			myAnim = STOP;
			if (myImg != nullptr)myImg->setPosOffset(0, 0);
			getContext()->getMngr()->setActive(getContext(), false);
		}

		break;
	default:
		break;
	}
}
