#include "PipePuzzleScene.h"
#include "PipePuzzleScene.h"
#include "PipePuzzleScene.h"

#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/RectArea2D.h"

PipePuzzleScene::PipePuzzleScene()
	:ScenePuzzleTemplate()
{
	
	
}

void PipePuzzleScene::pipeCreation()
{
	//type means if it needs two or 1 water connected to work
	pipeInfo pipeInfo0;
	pipeInfo0.waterAmount = 1;
	pipeInfo0.type = TWO;
	pipeInfo0.entry1 = <M, 0>;
	pipeInfo0.entry2 = <M, 1>;
	pipeInfo0.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo0));

	pipeInfo pipeInfo1;
	pipeInfo1.waterAmount = 1;
	pipeInfo1.type = ONE;
	pipeInfo1.entry1 = <M, 0>;
	pipeInfo1.entry2 = <P, 0>;
	pipeInfo1.result = true;

	_waterPipes.push_back(new Pipe(pipeInfo1));

	pipeInfo pipeInfo2;
	pipeInfo1.waterAmount = 0;
	pipeInfo1.type = TWO;
	pipeInfo1.entry1 = <P, 5>;
	pipeInfo1.entry2 = <P, 3>;
	pipeInfo1.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo2));

	pipeInfo pipeInfo3;
	pipeInfo3.waterAmount = 0;
	pipeInfo3.type = ONE;
	pipeInfo3.entry1 = <M, 1>;
	pipeInfo3.entry2 = <M, 4>;
	pipeInfo3.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo3));

	pipeInfo pipeInfo4;
	pipeInfo4.waterAmount = 0;
	pipeInfo4.type = TWO;
	pipeInfo4.entry1 = <M, 3>;
	pipeInfo4.entry2 = <M, 5>;
	pipeInfo4.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo4));

	pipeInfo pipeInfo5;
	pipeInfo5.waterAmount = 2;
	pipeInfo5.type = ONE;
	pipeInfo5.entry1 = <M, 4>;
	pipeInfo5.entry2 = <M, 5>;
	pipeInfo5.result = true;

	_waterPipes.push_back(new Pipe(pipeInfo5));

	pipeInfo pipeInfo6;
	pipeInfo5.waterAmount = 1;
	pipeInfo5.type = TWO;
	pipeInfo5.entry1 = <M, 3>;
	pipeInfo5.entry2 = <M, 2>;
	pipeInfo5.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo6));

	pipeInfo pipeInfo7;
	pipeInfo5.waterAmount = 0;
	pipeInfo5.type = ONE;
	pipeInfo5.entry1 = <P, 6>;
	pipeInfo5.entry2 = <M, 2>;
	pipeInfo5.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo7));


	pipeInfo pipeInfo8;
	pipeInfo5.waterAmount = 0;
	pipeInfo5.type = TWO;
	pipeInfo5.entry1 = <P, 4>;
	pipeInfo5.entry2 = <P, 7>;
	pipeInfo5.result = false;

	_waterPipes.push_back(new Pipe(pipeInfo8));

}

void PipePuzzleScene::moduleCreation()
{
	//result means if it carries water
	//dir means actual direction
	//down,right,left, up means what the module/pipe is closest to in that direction
	// P is pipe, M is module and N is null, the number means which one it is
	moduleInfo modInfo0;
	modInfo0.dir = RIGHT;
	modInfo0.down = <P, 0>;
	modInfo0.right = <P, 1>;
	modInfo0.left = <N, 0>;
	modInfo0.up = <N, 0>;
	modInfo0.result = true;

	_modules.push_back(new Module(modInfo0));

	moduleInfo modInfo1;
	modInfo1.dir = RIGHT;
	modInfo1.down = <P, 3>;
	modInfo1.right = <P, 0>;
	modInfo1.left = <N, 0>;
	modInfo1.up = <N, 0>;
	modInfo1.result = true;

	_modules.push_back(new Module(modInfo1));

	moduleInfo modInfo2;
	modInfo2.dir = DOWN;
	modInfo2.down = <P, 3>;
	modInfo2.right = <P, 0>;
	modInfo2.left = <P, 1>;
	modInfo2.up = <N, 0>;
	modInfo2.result = true;

	_modules.push_back(new Module(modInfo2));

	moduleInfo modInfo3;
	modInfo3.dir = DOWN;
	modInfo3.down = <P, 4>;
	modInfo3.right = <P, 6>;
	modInfo3.left = <P, 2>;
	modInfo3.up = <N, 0>;
	modInfo3.result = false;

	_modules.push_back(new Module(modInfo3));

	moduleInfo modInfo4;
	modInfo4.dir = RIGHT;
	modInfo4.down = <N, 0>;
	modInfo4.right = <P, 5>;
	modInfo4.left = <N, 0>;
	modInfo4.up = <P, 3>;
	modInfo4.result = true;

	_modules.push_back(new Module(modInfo4));


	moduleInfo modInfo5;
	modInfo4.dir = RIGHT;
	modInfo4.down = <N, 0>;
	modInfo4.right = <P, 4>;
	modInfo4.left = <N, 0>;
	modInfo4.up = <P, 5>;
	modInfo4.result = true;

	_modules.push_back(new Module(modInfo5));

}

void PipePuzzleScene::checkSolution()
{
	if (_waterPipes[8].getPipeInfo().waterAmount==2) //the last pipe has the solution
	{
		solved = true;
	}
}

Module::Directions PipePuzzleScene::getDirec(int module)
{
	return  _modules[module].getModuleInfo().dir;
}

void PipePuzzleScene::waterManagement(int beg, int end,int pipe, int operation)
{
	int half = beg + end / 2;

	if (half == pipe)
	{
		if (operation != -1 && _waterPipes[half].getPipeInfo().waterAmount != 0 ) //water cannot be <0
		{
			_waterPipes[half].changePipeInfo().waterAmount += operation;
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

void PipePuzzleScene::changeDirection(int module)
{
	if (_modules[module].getModuleInfo().dir == RIGHT)
	{
		_modules[module].changeModuleInfo().dir = DOWN;
	}
	else if (_modules[module].getModuleInfo().dir == DOWN &&( module==5|| module==4))
	{
		_modules[module].changeModuleInfo().dir = UP;
	}
	else if (_modules[module].getModuleInfo().dir == DOWN && (module != 5 && module != 4))
	{
		_modules[module].changeModuleInfo().dir = LEFT;
	}
	else if (_modules[module].getModuleInfo().dir == UP && (module == 5 || module == 4))
	{
		_modules[module].changeModuleInfo().dir = RIGHT;
	}
	else if (_modules[module].getModuleInfo().dir == LEFT && (module != 5 && module != 4))
	{
		_modules[module].changeModuleInfo().dir = UP;
	}

}


void PipePuzzleScene::init()
{
	solved = false;
	//creation of modules and pipes
	moduleCreation();
	pipeCreation();

	if (!isStarted) {

		//adding components to all modules

		//PIPES
		auto _module = entityManager->addEntity();
		auto _moduleTransform = entityManager->addComponent<Transform>(_module, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_module, &sdlutils().images().at("module"));
		entityManager->addComponent<RectArea2D>(_module);
		

		//MODULES
		auto _module5 = entityManager->addEntity();
		auto _moduleTransform5 = entityManager->addComponent<Transform>(_module5, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_module5, &sdlutils().images().at("module"));

		entityManager->addComponent<RectArea2D>(_module5);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_module5);
		clk->connect(ClickComponent::JUST_CLICKED, [this]() {changeDirection(5); });
		

	}
}

void PipePuzzleScene::waterFlow()
{
}

void PipePuzzleScene::waterArrives()
{
}

void PipePuzzleScene::waterLeaves()
{
}
