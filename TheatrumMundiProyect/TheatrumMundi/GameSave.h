#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
class GameSave
{
public:
    GameSave();
    GameSave(const std::string& filename);

    void Load(const std::string& filename);
    void Write(const std::string& filename);

    void setTutoCompleted(bool completed);
    bool isTutoCompleted();

protected:
    bool TutorialCompleted;
};