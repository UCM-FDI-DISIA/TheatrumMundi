#include "DialogueManager.h"
#include"../src/sdlutils/SDLUtils.h"
#include "../src/Components/Image.h"
#include "../src/ecs/Manager.h"
#include "EntityFactory.h"
#include "RectArea2D.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "../src/components/LogComponent.h"
#include "Log.h"
#include "SceneTemplate.h"
#include "../src/components/Transform.h"
#include "Area2DLayerManager.h"
#include "ClickableSpriteComponent.h"
#include "DialogueAnimComponent.h"


using namespace std;

DialogueManager::DialogueManager(int numRooms) : _scene(nullptr), displayOnProcess(false), characterimg(nullptr), _writeTextComp(nullptr){
    actualroom = numRooms;
    room = "Sala" + to_string(actualroom);
    dialogueReader = Game::Instance()->getReadDialogue();
   // _showText = TextInfo{ "", "" };
}

DialogueManager::~DialogueManager() {

}

void DialogueManager::Init(int numRooms,EntityFactory* entityFactory, EntityManager* entityManager, bool isMiddleRoom, Area2DLayerManager* areaLayerManager, string event)
{
    //if its middle room, character image is rendered first
    if (isMiddleRoom) {
        //Character (Image)
        //ENTIDADCONENTITYFACTORY
        auto character = entityFactory->CreateImageEntity(entityManager, "Dialog", Vector2D(500, 50), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0, ecs::grp::DIALOGUE);
        characterimg = entityManager->getComponent<Image>(character);
        character->getMngr()->addComponent<DialogueAnimComponent>(character);
        entityManager->setActive(character, false);
    }

    //Text Background
    //ENTIDADCONENTITYFACTORY
    auto _textbackground = entityFactory->CreateInteractableEntity(entityManager, "Dialog",EntityFactory::RECTAREA,Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, areaLayerManager,EntityFactory::NODRAG,ecs::grp::DIALOGUE);
    //No need to modify his transform at select
    entityManager->removeComponent<ClickableSpriteComponent>(_textbackground);
    auto dialogInteractionArea = entityManager->getComponent<RectArea2D>(_textbackground);
    entityManager->addComponent<DialogueAnimComponent>(_textbackground);
    // Put the dialog interaction area in front of the other interactables
    areaLayerManager->sendFront(dialogInteractionArea->getLayerPos());

    //Pass dialog if clicked
    auto clickTextDialgue = entityManager->getComponent<ClickComponent>(_textbackground);
    clickTextDialgue->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
        {
            if (!(Game::Instance()->getLog()->GetLogActive())) {
                //read dialogue only if it has to
                if (getDisplayOnProcess())
                {



                    ReadDialogue(_eventToRead);
                       
                    audioManager().stopSound(sdlutils().soundEffects().at("NextDialogue"));
                    audioManager().setVolume(sdlutils().soundEffects().at("NextDialogue"), float(0.1));
                    audioManager().playSound(sdlutils().soundEffects().at("NextDialogue"));
                }
                else
                {
                    _textbackground->getMngr()->setActive(_textbackground, false);
                }
            }
        });
    entityManager->addComponent<TriggerComponent>(_textbackground);
    _textbackground->getMngr()->addComponent<DialogueAnimComponent>(_textbackground);
    entityManager->setActive(_textbackground, false);

    //ENTIDADCONENTITYFACTORY
    //if it isnt middle room character image is rendered last
    if(!isMiddleRoom) 
    {
        //Character (Image)
        auto character = entityFactory->CreateImageEntity(entityManager, "Dialog", Vector2D(50, 310), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0, ecs::grp::DIALOGUE);
        characterimg = entityManager->getComponent<Image>(character);
        entityManager->addComponent<DialogueAnimComponent>(character);
        entityManager->setActive(character, false);
    }

    auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
    auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
    entityManager->addComponent<DialogueAnimComponent>(_textTest);
    entityManager->setActive(_textTest, false);

    //Add writeText to dialogueManager
    SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
    WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, &_showText);
    writeLogentityManager->setMiddleRoom(isMiddleRoom);

    _writeTextComp = writeLogentityManager;
}

void DialogueManager::setScene(SceneTemplate* scene)
{
	_scene = scene;
}

void DialogueManager::ReadDialogue(const string& event) {
    auto& roomDialogues = dialogueReader->getRoomDialogues(room);

    if (_writeTextComp->isFinished())
    {
        if (!roomDialogues[event].empty()) {
            
            displayOnProcess = true;
            _writeTextComp->startTextLine();

            TextInfo elem = roomDialogues[event].front();
            _showText.Character = elem.Character;
            _showText.Text = elem.Text;

            Game::Instance()->getLog()->addDialogueLineLog(elem.Character, elem.Text);
            setCharacterImage(elem.Character);

            roomDialogues[event].pop_front();

            //Indicate log the dialogue Event has ended
            if(roomDialogues[event].empty()) Game::Instance()->getLog()->addDialogueLineLog("/", "/");
        }
        else {
            
            for (auto a : _scene->GetEntityManager()->getEntities(ecs::grp::DIALOGUE)) {
                if (a->getMngr()->getComponent<DialogueAnimComponent>(a) != nullptr)
                    a->getMngr()->getComponent<DialogueAnimComponent>(a)->endDialogueAnim();

            }
            _scene->endDialogue();
            displayOnProcess = false;

            _showText.Character = " "; // Saves new text
            _showText.Text = " ";
            _writeTextComp->startTextLine();
        }
    }
    else
    {
        _writeTextComp->finishTextLine();
    }
    
}

void DialogueManager::setCharacterImage(const string& Character) {
    if (Character == "Keisara") characterimg->setTexture(&sdlutils().images().at("KeisaraSprite"));
    else if (Character == "Lucy") characterimg->setTexture(&sdlutils().images().at("LucySprite"));
    else if (Character == "Sol") characterimg->setTexture(&sdlutils().images().at("SolSprite"));
    else if (Character == " ") characterimg->setTexture(&sdlutils().images().at("EmptyImage")); //narrador
}

void DialogueManager::setEventToRead(std::string eventToRead)
{
    _eventToRead = eventToRead;
}

TextInfo DialogueManager::getShowText() {
    return _showText;
}

bool DialogueManager::getDisplayOnProcess() {
    return displayOnProcess;
}
