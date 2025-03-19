#pragma once
#include <list>
#include "EventsInfo.h"
#include "Manager.h"
#include "Inventory.h"
#include "Hint.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"

class Area2DLayerManager;
class SceneRoomTemplate;

class SceneTemplate
{
protected:

	 ecs::EntityManager* entityManager;
	 EntityFactory* entityFactory;
	// DialogueManager* dialogueManager;
	 Area2DLayerManager* areaLayerManager;
	/// <summary>
	/// Control is the scene has been iniciated or not. If(!isStarted) create all entities of the scece (this conditions is OBLIGATORY IN THE SCENE INIT)
	/// </summary>
	bool isStarted;
public:
	SceneTemplate();
	/// <summary>
	/// Create all the entities of the scene
	/// </summary>
	void virtual init() {};
	/// <summary>
	/// Create all the entities of the scene and recive a RoomScene*
	/// </summary>
	void virtual init(SceneRoomTemplate* s) {};
	/// <summary>
	/// Call the update() from all active entities 
	/// </summary>
	void update();
	/// <summary>
	/// Call the render() from all active entities 
	/// </summary>
	void render() const;
	/// <summary>
	/// Update de logic data of a scene
	/// </summary>
	void virtual refresh() {};
	/// <summary>
	/// DDEstroy all the entities of this scene
	/// </summary>
	void virtual unload() {};
	virtual ~SceneTemplate();
	/// <summary>
	/// Active de dialogue entities and pass a eventRoRead to DialogueManager
	/// </summary>
	/// <param name="_eventToRead"></param>
	void startDialogue(const eventToRead& _eventToRead);
	virtual void endDialogue();
	inline ecs::EntityManager* GetEntityManager() { return entityManager; }

};

