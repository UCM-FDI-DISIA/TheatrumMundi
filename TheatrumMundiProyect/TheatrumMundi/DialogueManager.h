#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include "ReadDialog.h"

#include "SceneTemplate.h"
#include <string>

class EntityFactory;
class Image;
class LogComponent;
class Area2DLayerManager;
class TextInfo;

class DialogueManager {
private:
    ReadDialog* dialogueReader;
    LogComponent* _sceneLog;
    SceneTemplate* _scene;
    Image* characterimg;
    TextInfo* _showText;
    bool displayOnProcess;
    int actualroom;
    std::string room;

public:
    DialogueManager(int numRooms);
    ~DialogueManager();

    void Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, bool isMiddleRoom, Area2DLayerManager* areaLayerManager, std::string _eventToRead);
    void ReadDialogue(const std::string& event);
    void setSceneLog(LogComponent* sceneLog);
    void setScene(SceneTemplate* scene);
    void setCharacterImage(const std::string& Character);

    TextInfo* getShowText();
    bool getDisplayOnProcess();
};

#endif
