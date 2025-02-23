#pragma once
#include <vector>
#include "ScenePuzzleTemplate.h"
#include "Pipe.h"
#include "Module.h"
using namespace std;
class Module;
class Pipe;
class PipePuzzleScene : public ScenePuzzleTemplate
{

private:
	//INFO//
	/*
	pipes with even index need 2 water sources, uneven only 1.
	pipes can only have 0,1,2 water connected.
	*/

	//vector de bools para tramos de agua
	//modules hacve 2 directions, posible 4 and actual 1
	// ref con pair a los  4 bjetos que tenga al lado y luego cada lado si tiene agua o no
	//con el pair te sacas el tipo por ejemplo mudulo 5 o pipe 4
	//el modulo tambien necesita un result para saber si lleva agua o no
	//para que un modulo heche agua necesita que el result de que le llege agua sea true y que la direccion sea correcta
	//los mudulos tienen 4 padres y los pipes tienen 2 
	//si hay un cambio en los results de hechar agua o no en los pipes o modulos toca cambiaar los que esten al lado
	struct Type {
		char elem;
		int num;
		Directions dir;
		
	};
	struct waterPath {

		int id;
		bool _withWater;
		Type _whoTocheck;
	};

	vector<waterPath>_waterPath;
	vector<Pipe*> _waterPipes; //vector that contains the amount of water each pipe has
	vector<Module*> _modules;//modules that change the direction of the water flow
	bool solved;

protected:


public: 
	 PipePuzzleScene();
	 void pipeCreation();
	 void moduleCreation();
	 void pathCreation();
	 bool Check() override;
	 void changeDirection(int module); //changes direction of the module
	 void init() override;
	 void waterPassPipe(int pipe); //if water can pass through a pipe 
	 void waterPassModule(int module); //if water has arrived to a certain module
	 void waterPassPath(int path);// series of conditions to check if paths have water
	 void unload() override;
	 ~PipePuzzleScene();
};

