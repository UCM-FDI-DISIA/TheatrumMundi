#include "Log.h"

#include "../src/ecs/Manager.h"
#include "EntityFactory.h"

#include"../src/sdlutils/SDLUtils.h"
#include "../src/Components/Image.h"

#include "RectArea2D.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "../src/components/LogComponent.h"

#include "SceneTemplate.h"
#include "../src/components/Transform.h"
#include "Area2DLayerManager.h"
#include "../TheatrumMundi/TextInfo.h"

#include "../src/components/WriteTextComponent.h"

#include "../src/Components/ScrollComponent.h"
#include "ClickableSpriteComponent.h"

#include "AudioManager.h"

using namespace std;

Log::Log()
{
	_firstRenderLine = _log.begin();
	sceneTemplate = nullptr;
}

//adds one dialogueLine (with its author) on log registry
void Log::addDialogueLineLog(std::string author, std::string dialogueLine)
{
	TextInfo aux;
	aux.Character = author;
	aux.Text = dialogueLine;
	_log.push_front(aux);
	_firstRenderLine = _log.begin();
}

void Log::cleanLogList()
{
	_log.clear();
	_firstRenderLine = _log.begin();
}

void Log::cleanRenderedList()
{
	_renderedDialogueLines.clear();
}

Log::~Log()
{
	_log.clear();
}

void Log::setRenderedDialogueLines()
{
	//temporal iterator
	auto logIt = _firstRenderLine;

	//verifies if iterator is at end
	if (logIt == _log.end()) { return;}
	else
	{
		//cleans list of rendering dialogue lines
		cleanRenderedList();

		//stores iterator's following 5 lines
		for (int i = 0; i < LINES_DISPLAYED; i++)
		{
			if (logIt == _log.end()) { return; }
			_renderedDialogueLines.push_back(*logIt);
			++logIt;
		}
	}
}

void Log::next()
{
	//distance between _firstRenderLine and list end
	int distanceToEnd = std::distance(_firstRenderLine, _log.end());

	if (distanceToEnd <= 5) return;

	int steps = std::min(5, distanceToEnd); //calculate how many steps _firstRenderLine has to advance
	std::advance(_firstRenderLine, steps);
}

void Log::previous()
{
	//distance between _firstRenderLine and list beginning
	int distanceToStart = std::distance(_log.begin(), _firstRenderLine);

	int steps = std::min(5, distanceToStart); //calculate how many steps _firstRenderLine has to de-advance
	std::advance(_firstRenderLine, -steps);
}

void Log::ResetLog()
{
	_log.clear();
	_renderedDialogueLines.clear();
	_firstRenderLine = _log.begin();
}

void Log::SetLogActive(bool logActive)
{
	_logActive = logActive;
}

bool Log::GetLogActive()
{
	return _logActive;
}

ecs::entity_t Log::Init(EntityFactory* entityFactory, EntityManager* entityManager, Area2DLayerManager* areaLayerManager, SceneTemplate* scTp)
{

	//Audio sfx
	AudioManager& a = AudioManager::Instance();
	Sound buttonSound = sdlutils().soundEffects().at("boton");
	a.setVolume(buttonSound, 0.2);

	sceneTemplate = scTp;
	//CREATE SCENE ENTITIES OF LOG

	//background log
	//ENTIDADCONENTITYFACTORY
	//auto _backgroundLog = entityFactory->CreateInteractableEntity(entityManager,"fondoPruebaLog",EntityFactory::RECTAREA,Vector2D(0,0),Vector2D(0,0),1349,748,0,areaLayerManager,EntityFactory::NODRAG,ecs::grp::LOG);
	//auto LogArea = entityManager->getComponent<RectArea2D>(_backgroundLog);
	//entityManager->removeComponent<ClickableSpriteComponent>(_backgroundLog);
	auto _backgroundLog = entityManager->addEntity(ecs::grp::LOG);
	entityManager->addComponent<Transform>(_backgroundLog, Vector2D(0, 0), Vector2D(0, 0), 1446, 768, 0); //transform
	auto imBack = entityManager->addComponent<Image>(_backgroundLog, &sdlutils().images().at("fondoPruebaLog")); //background log
	auto LogArea = entityManager->addComponent<RectArea2D>(_backgroundLog, areaLayerManager);
	entityManager->setActive(_backgroundLog, false);

	
	//text log
	//ENTIDADCONENTITYFACTORY
	auto _textLog = entityManager->addEntity(ecs::grp::LOG);
	Transform* trTextLog = entityManager->addComponent<Transform>(_textLog, Vector2D(0, 0), Vector2D(0, 0), 800, 600, 0);
	SDL_Color colorText = { 255, 255, 255, 255 };
	WriteTextComponent<std::list<TextInfo>>* writeLog =
		entityManager->addComponent<WriteTextComponent<std::list<TextInfo>>>(_textLog, sdlutils().fonts().at("BASE"), colorText, &_renderedDialogueLines); //write text component
	entityManager->setActive(_textLog, false);

	// Put the dialog interaction area in front of the other interactables
	areaLayerManager->sendFront(LogArea->getLayerPos());

	//scroll log buttons
	auto scrollDownLog = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1100, 80), Vector2D(0, 0), 80, 80, 270, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);
	auto scrollUpLog = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1100, 580), Vector2D(0, 0), 80, 80, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);
	
	ClickComponent* buttonScrollUpClick = entityManager->getComponent<ClickComponent>(scrollDownLog);
	buttonScrollUpClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
		AudioManager::Instance().playSound(buttonSound);
		previous();
		setRenderedDialogueLines();
		});
	entityManager->setActive(scrollDownLog, false);

	ClickComponent* buttonScrollDownClick = entityManager->getComponent<ClickComponent>(scrollUpLog);
	buttonScrollDownClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
		AudioManager::Instance().playSound(buttonSound);
		next();
		setRenderedDialogueLines();
		});
	entityManager->setActive(scrollUpLog, false);


	//log buttons
	entity_t buttonOpenLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(1200, 748 - (268 / 3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	auto buttonCloseLog = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(50, 50), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);

	ClickComponent* buttonOpenLogClick = entityManager->getComponent<ClickComponent>(buttonOpenLog);
	buttonOpenLogClick->connect(ClickComponent::JUST_CLICKED, [this, buttonCloseLog, buttonOpenLog, entityManager, buttonSound]() {
		setRenderedDialogueLines();
		AudioManager::Instance().playSound(buttonSound);
		//activate log
		auto _openButtonImage = entityManager->getComponent<Image>(buttonOpenLog);
		_openButtonImage->setW(entityManager->getComponent<Transform>(buttonOpenLog)->getWidth());
		_openButtonImage->setH(entityManager->getComponent<Transform>(buttonOpenLog)->getHeight());
		_openButtonImage->setPosOffset(0, 0);
		entityManager->setActiveGroup(ecs::grp::LOG, true);
		entityManager->setActive(buttonOpenLog, false); //close button
		_logActive = true;

		});
	entityManager->setActive(buttonOpenLog, true);

	ClickComponent* buttonCloseLogClick = entityManager->getComponent<ClickComponent>(buttonCloseLog);
	buttonCloseLogClick->connect(ClickComponent::JUST_CLICKED, [this, _backgroundLog, buttonCloseLog, buttonOpenLog, entityManager,buttonSound]() {
		_firstRenderLine = _log.begin();
		AudioManager::Instance().playSound(buttonSound);
		//disable log
		auto _backButtonImage = buttonCloseLog->getMngr()->getComponent<Image>(buttonCloseLog);
		_backButtonImage->setW(buttonCloseLog->getMngr()->getComponent<Transform>(buttonCloseLog)->getWidth());
		_backButtonImage->setH(buttonCloseLog->getMngr()->getComponent<Transform>(buttonCloseLog)->getHeight());
		_backButtonImage->setPosOffset(0, 0);
		entityManager->setActiveGroup(ecs::grp::LOG, false);
		entityManager->setActive(buttonOpenLog, true); //open button
		_logActive = false;
		sceneTemplate->closedLog();
		});
	entityManager->setActive(buttonCloseLog, false);
	return buttonOpenLog;
}
