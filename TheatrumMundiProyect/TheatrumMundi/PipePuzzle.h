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
	enum _directions { // diections a module can have

		DOWN,
		UP,
		RIGHT,
		LEFT
	};
	vector<pair<int,_directions>> _modules; //modules that change the direction of the water flow

protected:


public: 
	 PipePuzzle();
	 void waterManagement(int pipe, int operation); // add or decrease the amount of water connected to a certain pipe, (operation means +1 or -1)
	 void changeDirection(); //changes direction of the modules

};

