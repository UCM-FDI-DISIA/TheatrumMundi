#include "PipePuzzleScene.h"


#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "Direction.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/RectArea2D.h"
#include "../src/components/RectArea2D.h"

PipePuzzleScene::PipePuzzleScene()
	:ScenePuzzleTemplate()
{
	moduleCreation();
	pipeCreation();
	pathCreation();
	
}

void PipePuzzleScene::pipeCreation()
{
	int nextPipeId = 0;

	_waterPipes.push_back(new Pipe({ nextPipeId, TWO, {'M', 0, DOWN}, {'M', 1, RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, ONE, {'M', 0, RIGHT}, {'P', 0, NONE}, true }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, TWO, {'P', 5, NONE}, {'P', 3, NONE}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, ONE, {'M', 1, DOWN}, {'M', 4, UP}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, TWO, {'M', 3, DOWN}, {'M', 5, RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, ONE, {'M', 4, RIGHT}, {'M', 5, UP}, true }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, TWO, {'M', 3, RIGHT}, {'M', 2, DOWN}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, ONE, {'P', 6, NONE}, {'M', 2, RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, TWO, {'P', 4, NONE}, {'P', 7, NONE}, false }));


}

void PipePuzzleScene::moduleCreation()
{
	//result means if it carries water
	//dir means actual direction
	//down,right,left, up means what the module/pipe is closest to in that direction
	// P is pipe, M is module and N is null, the number means which one it is

	 int nextId = 0;
	
	_modules.push_back(new Module({ nextId, RIGHT, {'P', 1}, {'N', 1}, {'N', 0}, {'P', 0}, true }));
	_modules.push_back(new Module({ nextId++, RIGHT, {'P', 0}, {'N', 1}, {'N', 0}, {'P', 3}, true }));
	_modules.push_back(new Module({ nextId++, DOWN,  {'P', 0}, {'P', 1}, {'N', 0}, {'P', 3}, true }));
	_modules.push_back(new Module({ nextId++, DOWN,  {'P', 6}, {'P', 2}, {'N', 0}, {'P', 4}, false }));
	_modules.push_back(new Module({ nextId++, RIGHT, {'P', 5}, {'N', 0}, {'P', 3}, {'N', 1}, true }));
	_modules.push_back(new Module({ nextId++, RIGHT, {'P', 4}, {'N', 0}, {'P', 5}, {'N', 1}, true }));
}

void PipePuzzleScene::pathCreation()
{
    int nextId = 0;

	_waterPath.push_back({ nextId, true });  
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, false });
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, false }); 
	_waterPath.push_back({ nextId++, true });  
	
}

void PipePuzzleScene::Check()
{
	if (_waterPipes[8]->getPipeInfo().result =true) //the last pipe has the solution
	{
		solved = true;
	}
	else
	{
		solved= false;
	}
}

void PipePuzzleScene::changeDirection(int module)
{
	if (_modules[module]->getModuleInfo().dir == RIGHT)
	{
		_modules[module]->changeModuleInfo().dir = DOWN;
	}
	else if (_modules[module]->getModuleInfo().dir == DOWN &&( module==5|| module==4))
	{
		_modules[module]->changeModuleInfo().dir = UP;
	}
	else if (_modules[module]->getModuleInfo().dir == DOWN && (module != 5 && module != 4))
	{
		_modules[module]->changeModuleInfo().dir = LEFT;
	}
	else if (_modules[module]->getModuleInfo().dir == UP && (module == 5 || module == 4))
	{
		_modules[module]->changeModuleInfo().dir = RIGHT;
	}
	else if (_modules[module]->getModuleInfo().dir == LEFT && (module != 5 && module != 4))
	{
		_modules[module]->changeModuleInfo().dir = UP;
	}

	//
}

void PipePuzzleScene::init()
{
	solved = false;

	if (!isStarted) {

		//creamos vector con pos para pipes
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

		// Creamos los componentes de las pipes
		for (int i = 0; i < pipePositions.size(); ++i) {

			// creamos entidad
			auto pipeEntity = entityManager->addEntity();

			// añadimos el transfomr
			auto pipeTransform = entityManager->addComponent<Transform>(
				pipeEntity, pipePositions[i], Vector2D(0, 0), 500, 500, 0
			);

			// añadimos imagen
			entityManager->addComponent<Image>(pipeEntity, &sdlutils().images().at(""));

			// añadimos el área de visuallizado de la imagen
			entityManager->addComponent<RectArea2D>(pipeEntity);
		}


		//creamos vector con pos para modulos
		vector<Vector2D> modulePositions = {
			Vector2D(100, 0), // pos modulo 0
			Vector2D(200,0), // pos modulo 1
			Vector2D(300, 0), // pos modulo 2
			Vector2D(400, 0), // pos modulo 3
			Vector2D(0, 200), // pos modulo 4
			Vector2D(0, 200), // pos modulo 5
			Vector2D(0, 200), // pos modulo 6
			Vector2D(0, 200), // pos modulo 7
			Vector2D(0, 200)  // pos modulo 8
		};

		// Creamos los componentes de las pipes
		for (int i = 0; i < modulePositions.size(); ++i) {

			// creamos entidad
			auto moduleEntity = entityManager->addEntity();

			// añadimos el transfomr
			auto moduleTransform = entityManager->addComponent<Transform>(
				moduleEntity, modulePositions[i], Vector2D(0, 0), 500, 500, 0
			);

			// añadimos imagen
			entityManager->addComponent<Image>(moduleEntity, &sdlutils().images().at(""));

			// añadimos el área de visuallizado de la imagen
			entityManager->addComponent<RectArea2D>(moduleEntity);

			ClickComponent* clk = entityManager->addComponent<ClickComponent>(moduleEntity);
			clk->connect(ClickComponent::JUST_CLICKED, [this]() {
			Module* module = entityManager->getComponent<Module>(moduleEntity);
			changeDirection(module->getModuleInfo().id()); });
		}


		//creamos vector con pos para modulos
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

			// creamos entidad
			auto pathEntity = entityManager->addEntity();

			// añadimos el transfomr
			auto pathTransform = entityManager->addComponent<Transform>(
				pathEntity, pathPositions[i], Vector2D(0, 0), 500, 500, 0
			);

			// añadimos imagen
			entityManager->addComponent<Image>(pathEntity, &sdlutils().images().at(""));

			// añadimos el área de visuallizado de la imagen
			entityManager->addComponent<RectArea2D>(pathEntity);

		}

	}
}





void PipePuzzleScene::waterPassPipe(int pipe) {
	
	pipeInfo pipeData = _waterPipes[pipe]->changePipeInfo();

	// verificar si el tipo del tubo es ONE (1 entrada)
	if (pipeData.type == ONE) {
		// verificar si la entrada 1 tiene agua y cumple con la dirección
		if ((pipeData.entry1.id == 'P' && _waterPipes[pipeData.entry1.value]->getPipeInfo().result == true) ||
			(pipeData.entry1.id == 'M' && _modules[pipeData.entry1.value]->getModuleInfo().result == true &&
				_modules[pipeData.entry1.value]->getModuleInfo().dir == pipeData.entry1.direction)) // comprobar dir que tiene modulo es la misma que necesita la entrada
		{
			pipeData.result = true; // el tubo lleva agua
		}
		else {
			pipeData.result = false; // el tubo no lleva agua
		}
	}

	// verificar si el tipo del tubo es TWO (2 entradas)
	if (pipeData.type == TWO) {
		// comprobar si ambas entradas (entry1 y entry2) tienen agua y cumplen con las direcciones
		if ((pipeData.entry1.id == 'P' && _waterPipes[pipeData.entry1.value]->getPipeInfo().result == true) &&
			(pipeData.entry2.id == 'P' && _waterPipes[pipeData.entry2.value]->getPipeInfo().result == true) ||
			(pipeData.entry1.id == 'M' && _modules[pipeData.entry1.value]->getModuleInfo().result == true &&
				_modules[pipeData.entry1.value]->getModuleInfo().dir == pipeData.entry1.direction) &&
			(pipeData.entry2.id == 'M' && _modules[pipeData.entry2.value]->getModuleInfo().result == true &&
				_modules[pipeData.entry2.value]->getModuleInfo().dir == pipeData.entry2.direction)) // comprobar dir que tiene modulo es la misma que necesita la entrada
		{
			pipeData.result = true; // El tubo lleva agua
		}
		else {
			pipeData.result = false; // El tubo no lleva agua
		}
	}
}

void PipePuzzleScene::waterPassModule(int module) {

	moduleInfo modInfo = _modules[module]->changeModuleInfo();

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

	// nuevo estado actualizado de agua
	modInfo.result = receivesWater;
}

void PipePuzzleScene::waterPassPath()
{
	if (_module[0]->getModuleInfo().result && _module[0]->getModuleInfo().dir == RIGHT)
	{
		_waterPath[1]._withWater = true;
	}
	else
	{
		_waterPath[1]._withWater = false;
	}

	if (_module[1]->getModuleInfo().result && _module[1]->getModuleInfo().dir == RIGHT)
	{
		_waterPath[3]._withWater = true;
	}
	else
	{
		_waterPath[3]._withWater = false;
	}

	if (_waterPipes[0]->getPipeInfo().result==true)
	{
		_waterPath[4]._withWater = true;
	}
	else
	{
		_waterPath[4]._withWater = false;
	}

	if (_waterPipes[1]->getPipeInfo().result == true)
	{
		_waterPath[5]._withWater = true;
	}
	else
	{
		_waterPath[5]._withWater = false;
	}

	if (_module[2]->getModuleInfo().result && _module[2]->getModuleInfo().dir == RIGHT)
	{
		_waterPath[6]._withWater = true;
	}
	else
	{
		_waterPath[6]._withWater = false;
	}

	
}
