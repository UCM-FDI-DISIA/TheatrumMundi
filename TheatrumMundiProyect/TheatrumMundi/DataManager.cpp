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

    //0 = es posible
    // 1 y 2 = variantes de ha sido un mago 

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

DataManager::~DataManager()
{
}

void DataManager::ResetDataManager()
{
    ActualScene = MIDDLEROOM1;
    charactersAlive = { true,true,true };
    variants = { getRandomVariant(),getRandomVariant(),getRandomVariant() };
}
