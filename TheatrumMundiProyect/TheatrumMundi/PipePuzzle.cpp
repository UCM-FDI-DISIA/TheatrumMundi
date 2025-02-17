#include "PipePuzzle.h"

PipePuzzle::PipePuzzle(int modules, int pipes) 
	:ScenePuzzleTemplate()
{
	_amountPipes = pipes;

	_waterPipes[_amountPipes];
	_waterPipes[0] = 1;
	_waterPipes[1] = 1;
	_waterPipes[2] = 0;
	_waterPipes[3] = 0;
	_waterPipes[4] = 0;
	_waterPipes[5] = 2;
	_waterPipes[6] = 1;
	_waterPipes[7] = 0;
	_waterPipes[8] = 0;

	_modules[modules];
	_modules.push_back(RIGHT );
	_modules.push_back(DOWN  );
	_modules.push_back(RIGHT );
	_modules.push_back(DOWN  );
	_modules.push_back(RIGHT );
	_modules.push_back(RIGHT );

}

void PipePuzzle::checkSolution()
{
	if (_waterPipes[8] == 2) //the last pipe has the solution
	{
		solved = true;
	}
}

PipePuzzle::Directions PipePuzzle::getDirec(int module)
{
	return _modules[module];
}

void PipePuzzle::waterManagement(int beg, int end,int pipe, int operation)
{
	int half = beg + end / 2;

	if (half == pipe)
	{
		if (operation != -1 && _waterPipes[half] != 0 ) //water cannot be <0
		{
			_waterPipes[half] += operation;
		}
		
	}
	
	if (half < pipe) // search left
	{
		waterManagement(beg, half, pipe, operation);
	}
	else // search right
	{
		waterManagement(half, end, pipe, operation);
	}

}

void PipePuzzle::changeDirection(int module)
{
	if (_modules[module] == RIGHT)
	{
		_modules[module] = DOWN;
	}
	else if (_modules[module] == DOWN &&( module==5|| module==4))
	{
		_modules[module] = UP;
	}
	else if (_modules[module] == DOWN && (module != 5 && module != 4))
	{
		_modules[module] = LEFT;
	}
	else if (_modules[module] == UP && (module == 5 || module == 4))
	{
		_modules[module] = RIGHT;
	}
	else if (_modules[module] == LEFT && (module != 5 && module != 4))
	{
		_modules[module] = UP;
	}
}
