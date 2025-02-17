#pragma once
#include <vector>
#include "ScenePuzzleTemplate.h"
using namespace std;
class PipePuzzle:ScenePuzzleTemplate
{

private:
	//INFO//
	/*
	pipes with even index need 2 water sources, uneven only 1.
	pipes can only have 0,1,2 water connected.
	*/
	int _amountPipes; //how many pipes are there going to be
	vector<int> _waterPipes; //vector that contains the amount of water each pipe has
	enum Directions { // diections a module can have

		DOWN,
		UP,
		RIGHT,
		LEFT
	};
	vector<Directions> _modules; //modules that change the direction of the water flow

protected:


public: 
	 PipePuzzle(int modules, int pipes);
	 void checkSolution();
	 Directions getDirec(int module); 
	 void waterManagement(int beg, int end,int pipe, int operation); // add or decrease the amount of water connected to a certain pipe, (operation means +1 or -1)
	 void changeDirection(int module); //changes direction of the module

};

