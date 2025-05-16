#pragma once
#include <iostream>
#include <fstream>
#include <filesystem> 
#include <string>

class GameSave {
public:
    GameSave();
    ~GameSave();
    GameSave(const std::string& filename);

    void Load(const std::string& filename);
    void Write(const std::string& filename);

    void setTutoCompleted(bool completed);
    bool isTutoCompleted();


    static std::string GetSavePath(const std::string& filename);

protected:
    bool TutorialCompleted;
};