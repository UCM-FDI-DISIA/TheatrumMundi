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
}

//adds one dialogueLine (with its author) on log registry
void Log::addDialogueLineLog(std::string author, std::string dialogueLine)
{
	TextInfo aux;
	aux.Character = author;
	aux.Text = dialogueLine;
	_log.push_front(aux);
}

void Log::cleanLogList()
{
}

Log::~Log()
{
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
	

	//title log
	auto _titleLog = entityManager->addEntity(ecs::grp::LOG);
	entityManager->addComponent<Transform>(_titleLog, Vector2D(50, 50), Vector2D(0, 0), 100, 100, 0);
	entityManager->addComponent<Image>(_titleLog, &sdlutils().images().at("B7"));
	entityManager->setActive(_titleLog, false);

	//text log
	auto _textLog = entityManager->addEntity(ecs::grp::LOG);
	Transform* trTextLog = entityManager->addComponent<Transform>(_textLog, Vector2D(0, 0), Vector2D(0, 0), 800, 748, 0);
	Image* imTextLog = entityManager->addComponent<Image>(_textLog, &sdlutils().images().at("fondoPruebaLog"));
	SDL_Color colorText = { 255, 255, 255, 255 };
	WriteTextComponent<std::list<TextInfo>>* writeLog =
		entityManager->addComponent<WriteTextComponent<std::list<TextInfo>>>(_textLog, sdlutils().fonts().at("BASE"), colorText, &_log); //write text component
	entityManager->setActive(_textLog, false);

	// Put the dialog interaction area in front of the other interactables
	areaLayerManager->sendFront(LogArea->getLayerPos());

	//scroll log buttons
	
	auto scrollingLog = entityFactory->CreateInteractableEntityScroll(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1200, 100), Vector2D(0, 0), 50, 50, 270, areaLayerManager, sdlutils().height() / 50, 50, EntityFactory::SCROLLINVERSE, 0, EntityFactory::NODRAG, ecs::grp::LOG);
	auto upScrollingLog = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1200, 600), Vector2D(0, 0), 50, 50, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);
	auto ScrollComponentLog = entityManager->getComponent<ScrollComponent>(scrollingLog);
	entityManager->setActive(scrollingLog, false);
	entityManager->setActive(upScrollingLog, false);


	ScrollComponentLog->addElementToScroll(entityManager->getComponent<Transform>(_textLog));

	//log buttons
	auto buttonOpenLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(1200, 748 - (268 / 3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DIALOGUE);
	auto buttonCloseLog = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 500), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);

	ClickComponent* buttonOpenLogClick = entityManager->getComponent<ClickComponent>(buttonOpenLog);
	buttonOpenLogClick->connect(ClickComponent::JUST_CLICKED, [_titleLog, buttonCloseLog, buttonOpenLog, entityManager]() {
		//activate log
		entityManager->setActiveGroup(ecs::grp::LOG, true);
		entityManager->setActive(buttonOpenLog, false); //close button


		});
	entityManager->setActive(buttonOpenLog, true);

	ClickComponent* buttonCloseLogClick = entityManager->getComponent<ClickComponent>(buttonCloseLog);
	buttonCloseLogClick->connect(ClickComponent::JUST_CLICKED, [_backgroundLog, _titleLog, buttonCloseLog, buttonOpenLog, entityManager]() {
		//disable log
		entityManager->setActiveGroup(ecs::grp::LOG, false);
		entityManager->setActive(buttonOpenLog, true); //open button
		});
	entityManager->setActive(buttonCloseLog, false);

	
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
		});
	
	_textDialogueComp->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		while (ScrollComponentLog->numPhases() < (_log.size() / 5)) {
			ScrollComponentLog->addPhase();
		}
		});

		
}
