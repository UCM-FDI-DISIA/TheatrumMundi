#pragma once
#include <vector>
#include "ScenePuzzleTemplate.h"
using namespace std;
class PipePuzzleScene: public ScenePuzzleTemplate
{

private:
	//INFO//
	/*
	pipes with even index need 2 water sources, uneven only 1.
	pipes can only have 0,1,2 water connected.
	*/
	
	enum Type { // diections a module can have

		AND,//2 entries need to be true
		OR// 1 entrie needs to be true
	};
	struct Pipe
	{
		Type type;
		int _waterAmount;
		bool result;
	};

	//vector de bools para tramos de agua
	//modules hacve 2 directions, posible 4 and actual 1
	// ref con pair a los  4 bjetos que tenga al lado y luego cada lado si tiene agua o no
	//con el pair te sacas el tipo por ejemplo mudulo 5 o pipe 4
	//el modulo tambien necesita un result para saber si lleva agua o no
	//para que un modulo heche agua necesita que el result de que le llege agua sea true y que la direccion sea correcta
	//los mudulos tienen 4 padres y los pipes tienen 2 
	//si hay un cambio en los results de hechar agua o no en los pipes o modulos toca cambiaar los que esten al lado
	vector<Pipe> _waterPipes; //vector that contains the amount of water each pipe has
	enum Directions { // diections a module can have

		DOWN,
		UP,
		RIGHT,
		LEFT
	};

	vector<Directions> _modules; //modules that change the direction of the water flow

protected:


public: 
	 PipePuzzleScene(int modules, int pipes);
	 void checkSolution();
	 Directions getDirec(int module); 
	 void waterManagement(int beg, int end,int pipe, int operation); // add or decrease the amount of water connected to a certain pipe, (operation means +1 or -1)
	 void changeDirection(int module); //changes direction of the module
	 void init() override;
	 void waterFlow(); //if water can pass through a module to show it in an image
	 void waterArrives(); //if water has arrived to a certain module
	 void waterLeaves(); //if water has left a certain module

};

