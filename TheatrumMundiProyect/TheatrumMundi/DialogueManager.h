#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include "ReadDialog.h"
#include "../src/components/LogComponent.h"
#include "SceneTemplate.h"
#include "../src/Components/Image.h"

class DialogueManager {
private:
    ReadDialog* dialogueReader;
    LogComponent* _sceneLog;
    SceneTemplate* _scene;
    Image* characterimg;
    TextInfo* _showText;
    bool displayOnProcess;
    int actualroom;
    string room;

public:
    DialogueManager(int numRooms);
    ~DialogueManager();

    void ReadDialogue(const string& event);
    void setSceneLog(LogComponent* sceneLog);
    void setScene(SceneTemplate* scene);
    void setCharacterImage(const string& Character);

    TextInfo* getShowText();
    bool getDisplayOnProcess();
};

#endif
