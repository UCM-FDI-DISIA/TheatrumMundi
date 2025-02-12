#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
SceneTemplate::SceneTemplate()
{
	entityManager = new EntityManager();
	dialogueManager = new DialogueManager();

}

SceneTemplate::~SceneTemplate()
{
	unload();
	delete entityManager;
	delete dialogueManager;
}
