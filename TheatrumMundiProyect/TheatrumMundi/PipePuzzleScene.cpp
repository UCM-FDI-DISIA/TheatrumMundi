#include "PipePuzzleScene.h"


#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "Direction.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "../src/ecs/Entity.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/RectArea2D.h"
#include "../src/components/RectArea2D.h"


PipePuzzleScene::PipePuzzleScene()
	:ScenePuzzleTemplate()
{
	
}

void PipePuzzleScene::pipeCreation()
{
	int nextPipeId = 0;

	_waterPipes.push_back(new Pipe({ nextPipeId, Pipe::TWO, {'M', 0, DOWN}, {'M', 1, RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::ONE, {'M', 0,   RIGHT}, {'P', 0,  NONE}, true }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::TWO, {'P', 5,  NONE}, {'P', 3,  NONE}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++,Pipe::ONE, {'M', 1,  DOWN}, {'M', 4,  UP}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++,Pipe::TWO, {'M', 3,  DOWN}, {'M', 5,  RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++,Pipe::ONE, {'M', 4,   RIGHT}, {'M', 5,  UP}, true }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::TWO, {'M', 3,  RIGHT}, {'M', 2,  DOWN}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::ONE, {'P', 6,  NONE}, {'M', 2,  RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::TWO, {'P', 4,  NONE}, {'P', 7,  NONE}, false }));


}

void PipePuzzleScene::moduleCreation()
{
	// P is pipe, M is module and N is null, the number means which one it is
	//if N 1 means it is always going to have water
	//if N 0 never going to have water(null direction)
	 int nextId = 0;
	
	_modules.push_back(new Module({ nextId, RIGHT, {'P', 1}, {'N', 1}, {'N', 0}, {'P', 0}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 0}, {'N', 1}, {'N', 0}, {'P', 3}, true }));
	_modules.push_back(new Module({ nextId++,  DOWN,  {'P', 0}, {'P', 1}, {'N', 0}, {'P', 3}, true }));
	_modules.push_back(new Module({ nextId++,  DOWN,  {'P', 6}, {'P', 2}, {'N', 0}, {'P', 4}, false }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 5}, {'N', 0}, {'P', 3}, {'N', 1}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 4}, {'N', 0}, {'P', 5}, {'N', 1}, true }));
}

void PipePuzzleScene::pathCreation()
{
    int nextId = 0;

	_waterPath.push_back({ nextId, true, {'N',0,NONE}});//0
	_waterPath.push_back({ nextId++, false,{'M',0,RIGHT} });//1
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} }); //2
	_waterPath.push_back({ nextId++, true ,{'M',1,RIGHT} });//3
	_waterPath.push_back({ nextId++, false,{'P',0,NONE} });//4
	_waterPath.push_back({ nextId++, true,{'P',1,NONE} });//5
	_waterPath.push_back({ nextId++, false,{'M',2,RIGHT} });//6
	_waterPath.push_back({ nextId++, false,{'M',1,DOWN} });//7
	_waterPath.push_back({ nextId++, true ,{'M',4,UP} });//8
	_waterPath.push_back({ nextId++, false,{'P',3,NONE} });//9
	_waterPath.push_back({ nextId++, false,{'P',2,NONE} });//10
	_waterPath.push_back({ nextId++, true ,{'P',5,NONE} });//11
	_waterPath.push_back({ nextId++, false,{'M',3,RIGHT} });//12
	_waterPath.push_back({ nextId++, false,{'P',6,NONE} });//13
	_waterPath.push_back({ nextId++, false,{'P',7,NONE} });//14
	_waterPath.push_back({ nextId++, false ,{'P',4,NONE} });//15
	_waterPath.push_back({ nextId++, true ,{'M',5,RIGHT} });//16
	_waterPath.push_back({ nextId++, false,{'M',4,RIGHT} });//17
	_waterPath.push_back({ nextId++, false,{'N',0,NONE} });//18
	_waterPath.push_back({ nextId++, false,{'N',0,NONE} });//19
	_waterPath.push_back({ nextId++, true,{'M',5,UP} });//20
	_waterPath.push_back({ nextId++, false,{'P',8,NONE} });//21
	_waterPath.push_back({ nextId++, false,{'M',2,DOWN} });//22
	_waterPath.push_back({ nextId++, true,{'M',0,DOWN} });//23
	_waterPath.push_back({ nextId++, true ,{'M',3,DOWN} });//24
	
}

bool PipePuzzleScene::Check()
{
	if (_waterPipes[8]->getPipeInfo().result ==true) //the last pipe has the solution
	{
		solved = true;
		return true;
	}
	else
	{
		solved= false;
		return false;
	}
}

void PipePuzzleScene::changeDirection(int module)
{
	if (_modules[module]->getModuleInfo().dir == Direction::RIGHT)
	{
		_modules[module]->changeModuleInfo().dir = Direction::DOWN;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::DOWN && (module == 5 || module == 4))
	{
		_modules[module]->changeModuleInfo().dir = Direction::UP;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::DOWN && (module != 5 && module != 4))
	{
		_modules[module]->changeModuleInfo().dir = Direction::LEFT;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::UP && (module == 5 || module == 4))
	{
		_modules[module]->changeModuleInfo().dir = Direction::RIGHT;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::LEFT && (module != 5 && module != 4))
	{
		_modules[module]->changeModuleInfo().dir = Direction::UP;
	}
	
	//
}

void PipePuzzleScene::init()
{
	if (!isStarted) {
		solved = false;
		moduleCreation();
		pipeCreation();
		pathCreation();

		//vector with pipe positions
		vector<Vector2D> pipePositions = {
			Vector2D(100, 100), // pos pipe 0
			Vector2D(200, 100), // pos pipe 1
			Vector2D(300, 100), // pos pipe 2
			Vector2D(400, 100), // pos pipe 3
			Vector2D(100, 200), // pos pipe 4
			Vector2D(200, 200), // pos pipe 5
			Vector2D(300, 200), // pos pipe 6
			Vector2D(400, 200), // pos pipe 7
			Vector2D(500, 200)  // pos pipe 8
		};

		// creates the components of the pipe
		for (int i = 0; i < pipePositions.size(); ++i) {

			// create entity
			auto pipeEntity = entityManager->addEntity();

			// add transfomr
			auto pipeTransform = entityManager->addComponent<Transform>(
				pipeEntity, pipePositions[i], Vector2D(0, 0), 40, 40, 0
			);

			// add image
			entityManager->addComponent<Image>(pipeEntity, &sdlutils().images().at("exit"));

			Image* imageComponent = pipeEntity->getMngr()->getComponent<Image>(pipeEntity);

			if (_waterPipes[i]->getPipeInfo().type==Pipe::ONE)
			{
				//one entry
				imageComponent->setTexture(&sdlutils().images().at("1"));

			}
			else
			{
				//two entries
				imageComponent->setTexture(&sdlutils().images().at("2"));
			}
		}


		//vector with positions of modules
		vector<Vector2D> modulePositions = {
			Vector2D(100, 0), // pos module 0
			Vector2D(200,0), // pos module 1
			Vector2D(300, 0), // pos module 2
			Vector2D(400, 0), // pos module 3
			Vector2D(0, 200), // pos module 4
			Vector2D(0, 100), // pos module 5
		};


		// creates the components of the module
		for (int i = 0; i < modulePositions.size(); ++i) {

			// create entity
			auto moduleEntity = entityManager->addEntity();

			// add transfomr
			auto moduleTransform = entityManager->addComponent<Transform>(
				moduleEntity, modulePositions[i], Vector2D(0, 0), 100, 100, 0
			);

			// add image
			entityManager->addComponent<Image>(moduleEntity, &sdlutils().images().at("prueba"));

			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(moduleEntity);

			//add click component
			ClickComponent* clk = entityManager->addComponent<ClickComponent>(moduleEntity);
			clk->connect(ClickComponent::JUST_CLICKED, [this, i]() {std::cout << "name"<<i;
				changeDirection(_modules[i]->getModuleInfo().name);});

			Image* imageComponent = moduleEntity->getMngr()->getComponent<Image>(moduleEntity);

			if (_modules[i]->getModuleInfo().up.first=='N')
			{
				//module
				imageComponent->setTexture(&sdlutils().images().at("module"));

			}
			else
			{
				//module2
				imageComponent->setTexture(&sdlutils().images().at("module2"));
			}
		}


		//vector with path position
		vector<Vector2D> pathPositions = {
			Vector2D(100, 0), // pos modulo 0
			Vector2D(200,0), // pos modulo 1
			Vector2D(300, 0), // pos modulo 2
			Vector2D(400, 0), // pos modulo 3
			Vector2D(0, 200), // pos modulo 4
			Vector2D(0, 200), // pos modulo 5
			Vector2D(0, 200), // pos modulo 6
			Vector2D(0, 200), // pos modulo 7
			Vector2D(0, 200),  // pos modulo 8
			Vector2D(100, 0), // pos modulo 0
			Vector2D(200,0), // pos modulo 1
			Vector2D(300, 0), // pos modulo 2
			Vector2D(400, 0), // pos modulo 3
			Vector2D(0, 200), // pos modulo 4
			Vector2D(0, 200), // pos modulo 5
			Vector2D(0, 200), // pos modulo 6
			Vector2D(0, 200), // pos modulo 7
			Vector2D(0, 200) , // pos modulo 8
			Vector2D(100, 0), // pos modulo 0
			Vector2D(200,0), // pos modulo 1
			Vector2D(300, 0), // pos modulo 2
			Vector2D(400, 0), // pos modulo 3
		};


		for (int i = 0; i < pathPositions.size(); ++i) {

			// create entity
			 pathEntity = entityManager->addEntity();

			// add transform
			auto pathTransform = entityManager->addComponent<Transform>(
				pathEntity, pathPositions[i], Vector2D(0, 0), 60, 60, 0
			);

			// add image
			entityManager->addComponent<Image>(pathEntity, &sdlutils().images().at("init"));
			Image* imageComponent = pathEntity->getMngr()->getComponent<Image>(pathEntity);

			if (_waterPath[i]._withWater)
			{
				//texture with water
				imageComponent->setTexture(&sdlutils().images().at("pathWith"));

			}
			else
			{
				imageComponent->setTexture(&sdlutils().images().at("pathWithout"));
			}
			
			

		}

	}
}

void PipePuzzleScene::waterPassPipe(int pipe) {
	
	Pipe:: pipeInfo pipeData = _waterPipes[pipe]->changePipeInfo();

	if (pipeData.type ==Pipe:: ONE) {
		//checks if at least one entry (which can be a module or pipe) have water
		//if its a module it needs to check if the module's dir is the one the pipe needs for it to hace water
		if ((pipeData.entry1.name == 'P' && _waterPipes[pipeData.entry1.num]->getPipeInfo().result == true) ||
			(pipeData.entry1.name == 'M' && _modules[pipeData.entry1.num]->getModuleInfo().result == true &&
				(_modules[pipeData.entry1.num]->getModuleInfo().dir == pipeData.entry1.direction))) 
		{
			pipeData.result = true; 
		}
		else {
			pipeData.result = false;
		}
	}

	
	if (pipeData.type == Pipe:: TWO) {
		//checks if both entries (which can be a module or pipe) have water
		//if its a module it needs to check if the module's dir is the one the pipe needs for it to hace water
		if ((pipeData.entry1.name == 'P' && _waterPipes[pipeData.entry1.num]->getPipeInfo().result == true) &&
			(pipeData.entry2.name == 'P' && _waterPipes[pipeData.entry2.num]->getPipeInfo().result == true) ||
			(pipeData.entry1.name == 'M' && _modules[pipeData.entry1.num]->getModuleInfo().result == true &&
				_modules[pipeData.entry1.num]->getModuleInfo().dir == pipeData.entry1.direction) &&
			(pipeData.entry2.name == 'M' && _modules[pipeData.entry2.num]->getModuleInfo().result == true &&
				_modules[pipeData.entry2.num]->getModuleInfo().dir == pipeData.entry2.direction)) 
		{
			pipeData.result = true; 
		}
		else {
			pipeData.result = false; 
		}
	}
}

void PipePuzzleScene::waterPassModule(int module) {

	Module:: moduleInfo modInfo = _modules[module]->changeModuleInfo();

	bool receivesWater = false;

	if (modInfo.right.first == 'P' && _waterPipes[modInfo.right.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.right.first == 'M' && _modules[modInfo.right.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.right.first == 'N' && modInfo.right.second == 1) {
		receivesWater = true;
	}

	if (modInfo.left.first == 'P' && _waterPipes[modInfo.left.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.left.first == 'M' && _modules[modInfo.left.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.left.first == 'N' && modInfo.left.second == 1) {
		receivesWater = true;
	}

	if (modInfo.up.first == 'P' && _waterPipes[modInfo.up.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.up.first == 'M' && _modules[modInfo.up.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.up.first == 'N' && modInfo.up.second == 1) {
		receivesWater = true;
	}

	if (modInfo.down.first == 'P' && _waterPipes[modInfo.down.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.down.first == 'M' && _modules[modInfo.down.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.down.first == 'N' && modInfo.down.second == 1) {
		receivesWater = true;
	}

	
	modInfo.result = receivesWater;
}

void PipePuzzleScene::waterPassPath(int path)
{
	if (_waterPath[path]._whoTocheck.name == 'M')
	{
		if (_modules[_waterPath[path]._whoTocheck.num]->getModuleInfo().result && _modules[_waterPath[path]._whoTocheck.num]->getModuleInfo().dir == _waterPath[path]._whoTocheck.dir)
		{
			_waterPath[path]._withWater = true;
		}
		else
		{
			_waterPath[path]._withWater = false;
		}
	}
	else if (_waterPath[path]._whoTocheck.name == 'P')
	{
		if (_waterPipes[_waterPath[path]._whoTocheck.num]->getPipeInfo().result == true)
		{
			_waterPath[path]._withWater = true;
		}
		else
		{
			_waterPath[path]._withWater = false;
		}
	}
	else
	{
		_waterPath[path]._withWater = true;
	}

	Image* imageComponent = pathEntity->getMngr()->getComponent<Image>(pathEntity);

	if (_waterPath[path]._withWater)
	{
		//texture with water
		imageComponent->setTexture(&sdlutils().images().at("pathWith"));

	}
	else
	{
		imageComponent->setTexture(&sdlutils().images().at("pathWithout"));
	}
}

void PipePuzzleScene::unload()
{
	for (auto a : _waterPipes) delete a;
	for (auto a : _modules) delete a;
}

PipePuzzleScene::~PipePuzzleScene()
{
	unload();
}
