#include "GameSave.h"
#include <filesystem>
#include <windows.h>
#include <shlobj.h>

namespace fs = std::filesystem;

using namespace std;

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

GameSave::GameSave(const string& filename)
{
    Load(filename);
}

string GameSave::GetSavePath(const string& filename) { 
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath))) {
        fs::path savePath = fs::path(appDataPath) / "TheatrumMundi" / "saves";

        // Crear directorio si no existe
        if (!fs::exists(savePath)) {
            fs::create_directories(savePath);
        }

        return (savePath / filename).string();
    }

    // Fallback: directorio actual si no se puede obtener AppData
    fs::path fallbackPath = "saves";
    if (!fs::exists(fallbackPath)) {
        fs::create_directory(fallbackPath);
    }
    return (fallbackPath / filename).string();
}


void GameSave::Write(const std::string& filename)
{
    SaveData data;
    data.TutorialCompleted = TutorialCompleted;

    std::string fullPath = GetSavePath(filename);
    std::ofstream file(fullPath, std::ios::binary);

    if (file.is_open()) {
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
    std::string fullPath = GetSavePath(filename);
    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);

    TutorialCompleted = false; 

    if (!file.is_open()) return; 

    // Check file size
    std::streampos size = file.tellg();
    if (size != sizeof(SaveData)) {
        file.close();
        return; // Corrupt or empty file
    }

    file.seekg(0, std::ios::beg); // Back to top
    SaveData data;
    if (file.read(reinterpret_cast<char*>(&data), sizeof(SaveData))) {
        TutorialCompleted = data.TutorialCompleted;
    }

    file.close();
}