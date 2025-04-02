#pragma once
#include "ecs.h"

class EntityFactory;
class SceneTemplate;
class ClickComponent;
class Area2DLayerManager;

class PauseManager {
private:
	bool _pauseActive;
	SceneTemplate* _scene;
public:
	PauseManager();
	~PauseManager();
	void SetPauseActive(bool active);
	void setScene(SceneTemplate* scene);
	bool GetPauseActive();

	void Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager);
};