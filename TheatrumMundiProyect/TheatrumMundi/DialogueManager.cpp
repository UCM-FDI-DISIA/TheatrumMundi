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


using namespace std;

DialogueManager::DialogueManager(int numRooms) : _scene(nullptr), displayOnProcess(false), characterimg(nullptr), _writeTextComp(nullptr){
    actualroom = numRooms;
    room = "Sala" + to_string(actualroom);
    dialogueReader = new ReadDialog(numRooms);
    _showText = new TextInfo{ " ", " " };
}

DialogueManager::~DialogueManager() {
    delete _showText;
    delete dialogueReader;
}

void DialogueManager::Init(int numRooms,EntityFactory* entityFactory, EntityManager* entityManager, bool isMiddleRoom, Area2DLayerManager* areaLayerManager, string event)
{
    //if its middle room, character image is rendered first
    if (isMiddleRoom) {
        //Character (Image)
        auto character = entityManager->addEntity(grp::DIALOGUE);
        entityManager->addComponent<Transform>(character, Vector2D(500, 50), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0);
        characterimg = entityManager->addComponent<Image>(character, &sdlutils().images().at("Dialog"));

        entityManager->setActive(character, false);
    }

    //Text Background
    auto _textbackground = entityManager->addEntity(grp::DIALOGUE);
    entityManager->addComponent<Transform>(_textbackground, Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0);
    entityManager->addComponent<Image>(_textbackground, &sdlutils().images().at("Dialog"));
    auto dialogInteractionArea = entityManager->addComponent<RectArea2D>(_textbackground, areaLayerManager);
    // Put the dialog interaction area in front of the other interactables
    areaLayerManager->sendFront(dialogInteractionArea->getLayerPos());

    //Pass dialog if clicked
    auto clickTextDialgue = entityManager->addComponent<ClickComponent>(_textbackground);
    clickTextDialgue->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
        {
            if (!(Game::Instance()->getLog()->GetLogActive())) {
                //read dialogue only if it has to
                if (getDisplayOnProcess())
                {
                    ReadDialogue(_eventToRead);
                }
                else
                {
                    _textbackground->getMngr()->setActive(_textbackground, false);
                }
            }
        });
    entityManager->addComponent<TriggerComponent>(_textbackground);
    entityManager->setActive(_textbackground, false);

    //if it isnt middle room character image is rendered last
    if(!isMiddleRoom) 
    {
        //Character (Image)
        auto character = entityManager->addEntity(grp::DIALOGUE);
        entityManager->addComponent<Transform>(character, Vector2D(50, 310), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0);
        characterimg = entityManager->addComponent<Image>(character, &sdlutils().images().at("Dialog"));

        entityManager->setActive(character, false);
    }

    auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
    auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
    entityManager->setActive(_textTest, false);

    //Add writeText to dialogueManager
    SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
    WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, _showText);
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
            _showText->Character = elem.Character;
            _showText->Text = elem.Text;

            Game::Instance()->getLog()->addDialogueLineLog(elem.Character, elem.Text);
            setCharacterImage(elem.Character);

            roomDialogues[event].pop_front();
        }
        else {
            //Indicate log the dialogue Event has ended
            Game::Instance()->getLog()->addDialogueLineLog("/", "/");

            _scene->endDialogue();
            displayOnProcess = false;

            _showText->Character = " "; // Saves new text
            _showText->Text = " ";
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
    else if (Character == " ") characterimg->setTexture(&sdlutils().images().at("Hanni")); //narrador
}

void DialogueManager::setEventToRead(std::string eventToRead)
{
    _eventToRead = eventToRead;
}

TextInfo* DialogueManager::getShowText() {
    return _showText;
}

bool DialogueManager::getDisplayOnProcess() {
    return displayOnProcess;
}
