#include "DataManager.h"
#include "SDLUtils.h"

DataManager::DataManager()
{
    ActualScene = MIDDLEROOM1;
    charactersAlive = { true,true,true };
    variants = { getRandomVariant(),getRandomVariant(),getRandomVariant() };
}



int DataManager::getRandomVariant()

{
    //Room 1:
    //0 = posible
    // 1 and 2 = not posible

	//Room 2:
	//0 = not possible
	//1 and 2 = possible

	//Room 3:
    //0 = posible
    // 1 and 2 = not posible

    int mage = sdlutils().rand().nextInt(0,2);

    if (mage != 0) {

        mage = sdlutils().rand().nextInt(1, 3);

    }
     return mage;
    
    
}

void DataManager::ResetVariants()
{
    variants = { 0,0,0 };
}

void DataManager::SetSceneCount(SceneCount a)
{
    ActualScene = a;
}

void DataManager::SetCharacterDead(Character cha)
{
    charactersAlive[cha] = false;
}

int DataManager::GetRoomVariant(int i)
{
    return variants[i];
}

bool DataManager::GetCharacterState(Character cha)
{
    return charactersAlive[cha];
}

int DataManager::GetActualScene()
{
    return ActualScene;
}

void DataManager::SetRoom3Phase(int phase)
{
	room3phase = phase;
}

int DataManager::GetRoom3Phase()
{
    
    return room3phase;
}

DataManager::~DataManager()
{
}

void DataManager::ResetDataManager()
{
    ActualScene = MIDDLEROOM1;
    charactersAlive = { true,true,true };
    variants = { getRandomVariant(),getRandomVariant(),getRandomVariant() };
}
