#include "PipePuzzleScene.h"

#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/RectArea2D.h"

PipePuzzleScene::PipePuzzleScene(int modules, int pipes)
	:ScenePuzzleTemplate()
{

	_waterPipes.reserve(pipes);
	_waterPipes.push_back (1);
	_waterPipes.push_back(1);
	_waterPipes.push_back(0);
	_waterPipes.push_back(0);
	_waterPipes.push_back(0);
	_waterPipes.push_back(2);
	_waterPipes.push_back(1);
	_waterPipes.push_back(0);
	_waterPipes.push_back(0);

	_modules.reserve(modules);
	_modules.push_back(RIGHT );
	_modules.push_back(DOWN  );
	_modules.push_back(RIGHT );
	_modules.push_back(DOWN  );
	_modules.push_back(RIGHT );
	_modules.push_back(RIGHT );

}

void PipePuzzleScene::checkSolution()
{
	if (_waterPipes[8] == 2) //the last pipe has the solution
	{
		solved = true;
	}
}

PipePuzzleScene::Directions PipePuzzleScene::getDirec(int module)
{
	return _modules[module];
}

void PipePuzzleScene::waterManagement(int beg, int end,int pipe, int operation)
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

void PipePuzzleScene::changeDirection(int module)
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

	cout << _modules[module] << endl;
}

void PipePuzzleScene::init()
{
	cout << _modules[5] << endl;
	if (!isStarted) {
		auto _module5 = entityManager->addEntity();
		auto _moduleTransform5 = entityManager->addComponent<Transform>(_module5, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_module5, &sdlutils().images().at("module"));

		entityManager->addComponent<RectArea2D>(_module5);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_module5);
		clk->connect(ClickComponent::JUST_CLICKED, [this]() {changeDirection(5); });
		

	}
}