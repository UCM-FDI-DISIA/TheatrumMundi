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



using namespace std;

Log::Log(): _textDialogueComp(nullptr)
{
	_firstRenderLine = _log.begin();
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

		for (int i = 0; i < LINES_DISPLAYED; i++)
		{
			if (logIt == _log.end()) { return; }
			_renderedDialogueLines.push_back(*logIt);
			++logIt;
		}
	}
}

void Log::SetLogActive(bool logActive)
{
	_logActive = logActive;
}

bool Log::GetLogActive()
{
	return _logActive;
}

void Log::Init(EntityFactory* entityFactory, EntityManager* entityManager, Area2DLayerManager* areaLayerManager)
{
	//CREATE SCENE ENTITIES OF LOG

	//background log
	auto _backgroundLog = entityManager->addEntity(ecs::grp::LOG);
	entityManager->addComponent<Transform>(_backgroundLog, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
	auto imBack = entityManager->addComponent<Image>(_backgroundLog, &sdlutils().images().at("fondoPruebaLog")); //background log
	entityManager->setActive(_backgroundLog, false);

	auto LogArea = entityManager->addComponent<RectArea2D>(_backgroundLog, areaLayerManager);
	
	//text log
	auto _textLog = entityManager->addEntity(ecs::grp::LOG);
	Transform* trTextLog = entityManager->addComponent<Transform>(_textLog, Vector2D(0, 0), Vector2D(0, 0), 800, 748, 0);
	Image* imTextLog = entityManager->addComponent<Image>(_textLog, &sdlutils().images().at("fondoPruebaLog"));
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
	buttonScrollUpClick->connect(ClickComponent::JUST_CLICKED, [this]() {
		previous();
		setRenderedDialogueLines();
		});
	entityManager->setActive(scrollDownLog, false);

	ClickComponent* buttonScrollDownClick = entityManager->getComponent<ClickComponent>(scrollUpLog);
	buttonScrollDownClick->connect(ClickComponent::JUST_CLICKED, [this]() {
		next();
		setRenderedDialogueLines();
		});
	entityManager->setActive(scrollUpLog, false);



	//log buttons
	auto buttonOpenLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(1200, 748 - (268 / 3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	auto buttonCloseLog = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(50, 50), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);

	ClickComponent* buttonOpenLogClick = entityManager->getComponent<ClickComponent>(buttonOpenLog);
	buttonOpenLogClick->connect(ClickComponent::JUST_CLICKED, [this, buttonCloseLog, buttonOpenLog, entityManager]() {
		setRenderedDialogueLines();
		
		//activate log
		entityManager->setActiveGroup(ecs::grp::LOG, true);
		entityManager->setActive(buttonOpenLog, false); //close button


		});
	entityManager->setActive(buttonOpenLog, true);

	ClickComponent* buttonCloseLogClick = entityManager->getComponent<ClickComponent>(buttonCloseLog);
	buttonCloseLogClick->connect(ClickComponent::JUST_CLICKED, [_backgroundLog, buttonCloseLog, buttonOpenLog, entityManager]() {
		//disable log
		entityManager->setActiveGroup(ecs::grp::LOG, false);
		entityManager->setActive(buttonOpenLog, true); //open button
		});
	entityManager->setActive(buttonCloseLog, false);


	
	/*
	auto downScrollLogButton = entityManager->getComponent<ClickComponent>(scrollingLog);
	downScrollLogButton->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		std::cout << "A" << std::endl;
		if (!ScrollComponentLog->isScrolling()) {
			ScrollComponentLog->Scroll(ScrollComponent::DOWN);
		}
		});

	auto upScrollLogButton = entityManager->getComponent<ClickComponent>(upScrollingLog);
	upScrollLogButton->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		if (!ScrollComponentLog->isScrolling()) {
			ScrollComponentLog->Scroll(ScrollComponent::UP);
		}
		});
	
	buttonOpenLogClick->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		while (ScrollComponentLog->numPhases() < (_log.size() / 5)) {
			ScrollComponentLog->addPhase();
		}
		});*/
	
	/*
	_textDialogueComp->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		while (ScrollComponentLog->numPhases() < (_log.size() / 5)) {
			ScrollComponentLog->addPhase();
		}
		});*/

		
}
