#pragma once
#include <list>
#include<vector>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"


using namespace ecs;

enum SceneCount {
	MIDDLEROOM1 = 0,
	ROOM1,
	MIDDLEROOM2,
	ROOM2,
	MIDDLEROOM3,
	ROOM3,
	END	
};
class DataManager
{
private:
	SceneCount ActualScene;

	std::vector<int> variants;
	std::vector<bool> charactersAlive;

protected:
public:
	DataManager();

	int getRandomVariant();
	void ResetVariants();
	void SetSceneCount(SceneCount a);

	~DataManager();

};

