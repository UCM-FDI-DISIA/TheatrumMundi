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

DataManager::~DataManager()
{
}
