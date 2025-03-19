#include "DialogueManager.h"
#include"../src/sdlutils/SDLUtils.h"
#include "../src/Components/Image.h"
#include "../src/ecs/Manager.h"
#include "EntityFactory.h"
#include "RectArea2D.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "../src/components/LogComponent.h"
#include "../src/components/Transform.h"
#include "Area2DLayerManager.h"
#include "../TheatrumMundi/TextInfo.h"

#include "../src/components/WriteTextComponent.h"

using namespace std;

DialogueManager::DialogueManager(int numRooms) : _sceneLog(nullptr), _scene(nullptr), displayOnProcess(false), characterimg(nullptr) {
    actualroom = 1;
    room = "Sala" + to_string(actualroom);
    dialogueReader = new ReadDialog(numRooms);
    _showText = new TextInfo{ " ", " " };
}

DialogueManager::~DialogueManager() {
    delete _showText;
    delete dialogueReader;
}

void DialogueManager::Init(int numRooms,EntityFactory* entityFactory, EntityManager* entityManager, bool isMiddleRoom, Area2DLayerManager* areaLayerManager, string _eventToRead)
{
   
    if (isMiddleRoom) {
        //Character (Image)
        auto character = entityManager->addEntity(grp::DIALOGUE);
        entityManager->addComponent<Transform>(character, Vector2D(500, 50), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0);
        characterimg = entityManager->addComponent<Image>(character, &sdlutils().images().at("Dialog"));

        entityManager->setActive(character, false);

        
    }

    else {

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
    entityManager->addComponent<ClickComponent>(_textbackground)->connect(ClickComponent::JUST_CLICKED, [this, _textbackground, _eventToRead]()
        {
            if (!(_sceneLog->GetLogActive())) {
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


    auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
    auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
    entityManager->setActive(_textTest, false);

    //Add writeText to dialogueManager
    SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
    WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, _showText);
    

}

void DialogueManager::ReadDialogue(const string& event) {
    auto& roomDialogues = dialogueReader->getRoomDialogues(room);

    if (!roomDialogues[event].empty()) {
        TextInfo elem = roomDialogues[event].front();
        _showText->Character = elem.Character;
        _showText->Text = elem.Text;
        setCharacterImage(elem.Character);
        roomDialogues[event].pop_front();
    }
    else {
        _scene->endDialogue();
        displayOnProcess = false;
    }
}

void DialogueManager::setSceneLog(LogComponent* sceneLog) {
    _sceneLog = sceneLog;
}

void DialogueManager::setScene(SceneTemplate* scene) {
    _scene = scene;
}

void DialogueManager::setCharacterImage(const string& Character) {
    if (Character == "Keisara") characterimg->setTexture(&sdlutils().images().at("KeisaraSprite"));
    else if (Character == "Lucy") characterimg->setTexture(&sdlutils().images().at("LucySprite"));
    else if (Character == "Sol") characterimg->setTexture(&sdlutils().images().at("SolSprite"));
}

TextInfo* DialogueManager::getShowText() {
    return _showText;
}

bool DialogueManager::getDisplayOnProcess() {
    return displayOnProcess;
}
