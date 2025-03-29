#pragma once

class EntityFactory;
class SceneTemplate;

class PauseManager {
private:
	bool _pauseActive = false;
public:
	void SetPauseActive(bool active);
	bool GetPauseActive();

	void InitPause(EntityFactory* entityFactory, EntityManager* entityManager, Area2DLayerManager* areaLayerManager);
};