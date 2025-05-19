#define SDL_MAIN_HANDLED  
#include <SDL.h>
#include <windows.h>
#include "../../TheatrumMundiProyect/src/game/Game.h"

int main(int ac, char** av) {
    
#ifndef _DEBUG
    HWND console = GetConsoleWindow();
    if (console) ShowWindow(console, SW_HIDE);
#endif // !_DEBUG


    SDL_SetMainReady();

    try {
        Game::Instance()->init();
        Game::Instance()->start();
    }
    catch (const std::exception& e) {
       // MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR);
    }
    catch (...) {
        MessageBoxA(NULL, "Error desconocido", "Error", MB_ICONERROR);
    }

    delete Game::Instance();
    return 0;
}