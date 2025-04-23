#include "GameSave.h"

struct SaveData {
    bool TutorialCompleted;
    // Add more data
    //...
    //...
    // etc...
};

GameSave::GameSave() : TutorialCompleted(false) {}

GameSave::~GameSave()
{
}

GameSave::GameSave(const std::string& filename)
{
    Load(filename);
}

void GameSave::Write(const std::string& filename)
{
    SaveData data;
    data.TutorialCompleted = TutorialCompleted;
    //Add more data

    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char*>(&data), sizeof(SaveData));
        file.close();
    }
}

void GameSave::setTutoCompleted(bool completed)
{
    TutorialCompleted = completed;
}

bool GameSave::isTutoCompleted()
{
    return TutorialCompleted;
}

void GameSave::Load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        SaveData data;
        file.read(reinterpret_cast<char*>(&data), sizeof(SaveData));
        TutorialCompleted = data.TutorialCompleted;
        if(TutorialCompleted)
        // Load more data
        file.close();
    }
    else {

        TutorialCompleted = false;
    }
}