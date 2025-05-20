#include <iostream>
#include "../../TheatrumMundiProyect/src/game/Game.h"

int main(int ac, char** av) {
	try {

		Game::Instance()->init();
		Game::Instance()->start();
	}
	catch (const std::string& e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	}
	catch (const char* e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}
	delete Game::Instance();
	return 0;
}

/*
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
		MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(NULL, "Error desconocido", "Error", MB_ICONERROR);
	}

	delete Game::Instance();
	return 0;
}
*/