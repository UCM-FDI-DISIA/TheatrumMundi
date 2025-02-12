#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"


SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();

}

void SceneTemplate::render() const
{
	entityManager->render();
}

SceneTemplate::~SceneTemplate()
{
	unload();
	delete entityManager;
//	delete dialogueManager;
}
