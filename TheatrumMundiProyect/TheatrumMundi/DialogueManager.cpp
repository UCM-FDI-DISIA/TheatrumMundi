#include "DialogueManager.h"
#include"../src/sdlutils/SDLUtils.h"

DialogueManager::DialogueManager(int numRooms) : _sceneLog(nullptr), _scene(nullptr), displayOnProcess(false) {
    actualroom = 1;
    room = "Sala" + to_string(actualroom);
    dialogueReader = new ReadDialog(numRooms);
    _showText = new TextInfo{ " ", " " };
}

DialogueManager::~DialogueManager() {
    delete _showText;
    delete dialogueReader;
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
