#include "DialogueAnimComponent.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../Components/Transform.h"
#include "../Components/Image.h"
#include "../sdlutils/SDLUtils.h"
#include "../src/game/Game.h"
#include "WriteTextComponent.h"

DialogueAnimComponent::DialogueAnimComponent(int movespeed , int alphaspeed)
{
	_movespeed = movespeed;
	_alphaspeed = alphaspeed;
	myImg = nullptr;
	myTransform = nullptr;
	_mytext = nullptr;
	myAnim = STOP;
	distantoToanim = 500 * Game::Instance()->hscreenScale;
}

DialogueAnimComponent::~DialogueAnimComponent()
{
}

void DialogueAnimComponent::startDialogueAnim()
{
	myAnim = START;
	myTransform = getContext()->getMngr()->getComponent<Transform>(getContext());
	_mytext = getContext()->getMngr()->getComponent<WriteTextComponent<TextInfo>>(getContext());
	myImg = getContext()->getMngr()->getComponent<Image>(getContext());
	originalPos = myTransform->getPos();
	posCooldown = distantoToanim;
	actmovespeed = _movespeed;
	if(myImg != nullptr)myImg->setPosOffset(Vector2D(0,distantoToanim));
	if (_mytext != nullptr)_mytext->addOffset(0, distantoToanim);
}

void DialogueAnimComponent::endDialogueAnim()
{
	myAnim = END;
	posCooldown = 0;
	actmovespeed = _movespeed;
}

void DialogueAnimComponent::update()
{
	switch (myAnim)
	{
	case DialogueAnimComponent::STOP:
		break;
	case DialogueAnimComponent::START:
		
		if (myImg != nullptr)myImg->addPosOffset(0, -actmovespeed );
		if(_mytext !=nullptr) _mytext->addOffset(0,-actmovespeed );
			posCooldown -= actmovespeed;
		if (actmovespeed < maxSpeed) actmovespeed+=1;
	
		if (posCooldown < 0) {
			myTransform->setPosPure(originalPos);
			if (_mytext != nullptr) _mytext->resetOffset();
			if (myImg != nullptr)myImg->setPosOffset(0, 0);
			myAnim = STOP;
		}
		break;
	case DialogueAnimComponent::END:
		if (myImg != nullptr)myImg->addPosOffset(0, +actmovespeed);
		if (_mytext != nullptr) _mytext->addOffset(0, actmovespeed );
		posCooldown += actmovespeed;
		if (actmovespeed < maxSpeed) actmovespeed += 1;

		if (posCooldown > (distantoToanim)) {
			//myTransform->setPosPure(originalPos);
			myAnim = STOP;
			if (myImg != nullptr)myImg->setPosOffset(0, 0);
			if (_mytext != nullptr) _mytext->resetOffset();
			getContext()->getMngr()->setActive(getContext(), false);
		}

		break;
	default:
		break;
	}
}
