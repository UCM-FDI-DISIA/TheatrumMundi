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
enum Character {
	KEISARA = 0,
	SOL,
	LUCY
};

class DataManager
{
private:

	SceneCount ActualScene;

	std::vector<int> variants;// rooms 
	std::vector<bool> charactersAlive;
	int room3phase = 0;

protected:
public:
	
	DataManager();

	int getRandomVariant();
	void ResetVariants();
	void SetSceneCount(SceneCount a);
	void SetCharacterDead(Character cha);

	int GetRoomVariant(int i);
	bool GetCharacterState(Character cha);
	int GetActualScene();

	
	void SetRoom3Phase(int phase);
	int GetRoom3Phase();

	~DataManager();

	void ResetDataManager();
};

