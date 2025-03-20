#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include "ReadDialog.h"


#include <string>

#include "ecs.h"

class SceneTemplate;
class EntityFactory;
class Image;
class Log;
class Area2DLayerManager;
class TextInfo;

class DialogueManager {
private:
    ReadDialog* dialogueReader;
    SceneTemplate* _scene;
    Image* characterimg;
    TextInfo* _showText;
    bool displayOnProcess;
    int actualroom;
    std::string room;

public:
    DialogueManager(int numRooms);
    ~DialogueManager();

    void Init(int numRooms,EntityFactory* entityFactory, ecs::EntityManager* entityManager, bool isMiddleRoom, Area2DLayerManager* areaLayerManager, std::string _eventToRead);
    void ReadDialogue(const std::string& event);
    void setScene(SceneTemplate* scene);
    void setCharacterImage(const std::string& Character);
    TextInfo* getShowText();
    inline void setdisplayOnProcess(bool disply) { displayOnProcess = disply; }
    bool getDisplayOnProcess();
};

#endif
