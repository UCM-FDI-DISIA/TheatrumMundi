#include "PipePuzzleScene.h"
#include "EntityFactory.h"

#include "../src/utils/Vector2D.h";

#include "../src/game/Game.h";
#include "../src/components/Transform.h"
#include "Direction.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "../src/ecs/Entity.h"
#include "../TheatrumMundi/SceneManager.h"

#include "../src/components/ClickComponent.h"
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
	_modules.push_back(new Module({ nextId++,  DOWN,  {'P', 7}, {'P', 1}, {'N', 0}, {'P', 6}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT,  {'P', 6}, {'P', 2}, {'N', 0}, {'P', 4}, false }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 5}, {'N', 0}, {'P', 3}, {'N', 1}, true }));
	_modules.push_back(new Module({ nextId++,  UP, {'P', 4}, {'N', 0}, {'P', 5}, {'N', 1}, true }));
}

void PipePuzzleScene::pathCreation()
{
    int nextId = 0;

	
	_waterPath.push_back({ nextId, true, {'N',0,NONE}});//0
	auto pathp1 = entityFactory->CreateImageEntity(entityManager, "exit", EntityFactory::RECTAREA, Vector2D(100, 100),
	Vector2D(0, 0), 50, 20, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ pathp1, 1 });

	_waterPath.push_back({ nextId, true, {'N',0,NONE} });//0
	auto pathp1 = entityFactory->CreateImageEntity(entityManager, "exit", EntityFactory::RECTAREA, Vector2D(100, 100),
		Vector2D(0, 0), 50, 20, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ pathp1, 1 });


	_waterPath.push_back({ nextId, true, {'N',0,NONE} });//0
	auto pathp1 = entityFactory->CreateImageEntity(entityManager, "exit", EntityFactory::RECTAREA, Vector2D(100, 100),
		Vector2D(0, 0), 50, 20, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ pathp1, 1 });

	_waterPath.push_back({ nextId, true, {'N',0,NONE} });//0
	auto pathp1 = entityFactory->CreateImageEntity(entityManager, "exit", EntityFactory::RECTAREA, Vector2D(100, 100),
		Vector2D(0, 0), 50, 20, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ pathp1, 1 });

	_waterPath.push_back({ nextId, true, {'N',0,NONE} });//0
	auto pathp1 = entityFactory->CreateImageEntity(entityManager, "exit", EntityFactory::RECTAREA, Vector2D(100, 100),
		Vector2D(0, 0), 50, 20, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ pathp1, 2 });  


	_waterPath.push_back({ nextId, true, {'N',0,NONE} });//0
	auto pathp1 = entityFactory->CreateImageEntity(entityManager, "exit", EntityFactory::RECTAREA, Vector2D(100, 100),
		Vector2D(0, 0), 50, 20, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ pathp1, 1 });











	_waterPath.push_back({ nextId++, true,{'M',0,RIGHT} });//1
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} }); //2
	_waterPath.push_back({ nextId++, true ,{'M',1,RIGHT} });//3
	_waterPath.push_back({ nextId++, false,{'P',0,NONE} });//4
	_waterPath.push_back({ nextId++, true,{'P',1,NONE} });//5
	_waterPath.push_back({ nextId++, false,{'M',2,RIGHT} });//6
	_waterPath.push_back({ nextId++, false,{'M',1,DOWN} });//7
	_waterPath.push_back({ nextId++, false ,{'M',4,UP} });//8
	_waterPath.push_back({ nextId++, false,{'P',3,NONE} });//9
	_waterPath.push_back({ nextId++, false,{'P',2,NONE} });//10
	_waterPath.push_back({ nextId++, false ,{'P',5,NONE} });//11
	_waterPath.push_back({ nextId++, false,{'M',3,RIGHT} });//12
	_waterPath.push_back({ nextId++, false,{'P',6,NONE} });//13
	_waterPath.push_back({ nextId++, false,{'P',7,NONE} });//14
	_waterPath.push_back({ nextId++, false ,{'P',4,NONE} });//15
	_waterPath.push_back({ nextId++, false ,{'M',5,RIGHT} });//16
	_waterPath.push_back({ nextId++, true,{'M',4,RIGHT} });//17
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} });//18
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} });//19
	_waterPath.push_back({ nextId++, true,{'M',5,UP} });//20
	_waterPath.push_back({ nextId++, false,{'P',8,NONE} });//21
	_waterPath.push_back({ nextId++, true,{'M',2,DOWN} });//22
	_waterPath.push_back({ nextId++, false,{'M',0,DOWN} });//23
	_waterPath.push_back({ nextId++, false ,{'M',3,DOWN} });//24
	
	
}

bool PipePuzzleScene::Check()
{
	if (_waterPipes[8]->getPipeInfo().result ==true) //the last pipe has the solution
	{
		solved = true;
		Win();
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
	//changes the direction +90º

	if (_modules[module]->getModuleInfo().dir == Direction::RIGHT)
	{
		_modules[module]->changeModuleInfo().dir = Direction::DOWN;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::DOWN )
	{
		_modules[module]->changeModuleInfo().dir = Direction::LEFT;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::UP )
	{
		_modules[module]->changeModuleInfo().dir = Direction::RIGHT;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::LEFT )
	{
		_modules[module]->changeModuleInfo().dir = Direction::UP;
	}

	//cout<<"DIR"<<module<<" "<< _modules[module]->getModuleInfo().dir<<endl;
	//rotates the module
	Transform* transformComponent = _modulesEnt[module]->getMngr()->getComponent<Transform>(_modulesEnt[module]);
	
	float newRot = transformComponent->getRot() + 90.0f;
	if (newRot >= 360.0f) {
		newRot -= 360.0f;
	}
	transformComponent->setRot(newRot); 

	updatePuzzle();
	
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
		for (int i = 0; i < pipePositions.size(); i++) {

			// create entity
			_pipesEnt.push_back(entityManager->addEntity());
			
			// add transfomr
			auto pipeTransform = entityManager->addComponent<Transform>(
				_pipesEnt[i], pipePositions[i], Vector2D(0, 0), 40, 40, 0
			);

			// add image
			entityManager->addComponent<Image>(_pipesEnt[i], &sdlutils().images().at("exit"));

			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(_pipesEnt[i]);

			Image* imageComponent = _pipesEnt[i]->getMngr()->getComponent<Image>(_pipesEnt[i]);

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
			Vector2D(400,0), // pos module 0
			Vector2D(100, 100), // pos module 1
			Vector2D(1000, 100), // pos module 2
			Vector2D(800,300), // pos module 3
			Vector2D(50,400), // pos module 4
			Vector2D(250,600), // pos module 5
		};


		// creates the components of the module
		for (int i = 0; i < modulePositions.size(); i++) {

			// create entity
			_modulesEnt.push_back(entityManager->addEntity());
	

			// add transfomr
			auto moduleTransform = entityManager->addComponent<Transform>(
				_modulesEnt[i], modulePositions[i], Vector2D(0, 0), 100, 100, 0
			);

			// add image
			entityManager->addComponent<Image>(_modulesEnt[i], &sdlutils().images().at("prueba"));

			Transform* transformComponent = _modulesEnt[i]->getMngr()->getComponent<Transform>(_modulesEnt[i]);

			if (i == 2)
			{
				transformComponent->setRot(transformComponent->getRot() + 90.0f);
			}
			else if (i == 5)
			{
				transformComponent->setRot(transformComponent->getRot() + 270.0f);
			}
			
			

			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(_modulesEnt[i]);

			//add click component
			ClickComponent* clk = entityManager->addComponent<ClickComponent>(_modulesEnt[i]);
			clk->connect(ClickComponent::JUST_CLICKED, [this, i]() {
				changeDirection(i); 
				});

			Image* imageComponent = _modulesEnt[i]->getMngr()->getComponent<Image>(_modulesEnt[i]);

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


/*		vector<Vector2D> pathPositions = {
	Vector2D(50, 50),
	Vector2D(150, 50),
	Vector2D(250, 50),
	Vector2D(350, 50),
	Vector2D(450, 50),
	Vector2D(550, 50),
	Vector2D(650, 50),
	Vector2D(750, 50),

	Vector2D(50, 150),
	Vector2D(150, 150),
	Vector2D(250, 150),
	Vector2D(350, 150),
	Vector2D(450, 150),
	Vector2D(550, 150),
	Vector2D(650, 150),
	Vector2D(750, 150),

	Vector2D(50, 250),
	Vector2D(150, 250),
	Vector2D(250, 250),
	Vector2D(350, 250),
	Vector2D(450, 250),
	Vector2D(550, 250),
	Vector2D(650, 250),
	Vector2D(750, 250),
	Vector2D(400, 350)
		};



		for (int i = 0; i < pathPositions.size(); i++) {

			// create path
			_pathEnt.push_back(entityManager->addEntity());
			

			// add transform
			auto pathTransform = entityManager->addComponent<Transform>(
				_pathEnt[i], pathPositions[i], Vector2D(0, 0), 60, 60, 0
			);

			cout << "CARGADO" << i << endl;
			// add image
			entityManager->addComponent<Image>(_pathEnt[i], &sdlutils().images().at("init"));
			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(_pathEnt[i]);
			Image* imageComponent = _pathEnt[i]->getMngr()->getComponent<Image>(_pathEnt[i]);

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
	*/
		// create cube
		auto cubeEntity = entityManager->addEntity();

		// add transfomr
		auto cubeTransform = entityManager->addComponent<Transform>(
			cubeEntity, Vector2D(900,580), Vector2D(0, 0), 150, 150, 0
		);
		// add image
		entityManager->addComponent<Image>(cubeEntity, &sdlutils().images().at("cube"));

		// add area of visualization of the image
		entityManager->addComponent<RectArea2D>(cubeEntity);

		//create exit botton
		auto exitEntity = entityManager->addEntity();

		// add transfomr
		auto exitTransform = entityManager->addComponent<Transform>(
			exitEntity, Vector2D(50, 580), Vector2D(0, 0), 150, 150, 0
		);
		// add image
		entityManager->addComponent<Image>(exitEntity, &sdlutils().images().at("exit"));

		// add area of visualization of the image
		entityManager->addComponent<RectArea2D>(exitEntity);




	}
}

void PipePuzzleScene::waterPassPipe(int pipe) {

	
	Pipe::pipeInfo pipeData = _waterPipes[pipe]->getPipeInfo();  

	//checks if its type 1 pipe only 1 entry has to have water not 2
	if (pipeData.type == Pipe::ONE) {
		
		//the entry(neightbourd) can be a module or a pipe so if its a module it needs to have the direction the entry needs
		bool ent1 = (pipeData.entry1.name == 'P' && _waterPipes[pipeData.entry1.num]->getPipeInfo().result == true) ||
			(pipeData.entry1.name == 'M' && _modules[pipeData.entry1.num]->getModuleInfo().result == true &&
				_modules[pipeData.entry1.num]->getModuleInfo().dir == pipeData.entry1.direction);

		bool ent2 = (pipeData.entry2.name == 'P' && _waterPipes[pipeData.entry2.num]->getPipeInfo().result == true) ||
			(pipeData.entry2.name == 'M' && _modules[pipeData.entry2.num]->getModuleInfo().result == true &&
				_modules[pipeData.entry2.num]->getModuleInfo().dir == pipeData.entry2.direction);

		//if the condition is correct change the result(if it carries water) to true
		if ((ent1 && !ent2)|| (!ent1 &&ent2))
		
		{  
			_waterPipes[pipe]->changePipeInfo().result = true;
				
			
		}
		else {
			
			_waterPipes[pipe]->changePipeInfo().result = true;
			
			
		}
	}

	//this type of pipe needs to entries with water
	if (pipeData.type == Pipe::TWO) {
		//the entry(neightbourd) can be a module or a pipe so if its a module it needs to have the direction the entry needs
		bool entry1HasWater = (pipeData.entry1.name == 'P' && _waterPipes[pipeData.entry1.num]->getPipeInfo().result) ||
			(pipeData.entry1.name == 'M' && _modules[pipeData.entry1.num]->getModuleInfo().result &&
				_modules[pipeData.entry1.num]->getModuleInfo().dir == pipeData.entry1.direction);

		bool entry2HasWater = (pipeData.entry2.name == 'P' && _waterPipes[pipeData.entry2.num]->getPipeInfo().result) ||
			(pipeData.entry2.name == 'M' && _modules[pipeData.entry2.num]->getModuleInfo().result &&
				_modules[pipeData.entry2.num]->getModuleInfo().dir == pipeData.entry2.direction);

		//if the condition is correct change the result(if it carries water) to true
		if (entry1HasWater && entry2HasWater) {
			 
			_waterPipes[pipe]->changePipeInfo().result = true;
				
		}
		else {
			 
			_waterPipes[pipe]->changePipeInfo().result = false;
			
		}
	}

	
	//std::cout << "Pipe " << pipe << " tiene resultado: " << pipeData.result << std::endl;

}

void PipePuzzleScene::waterPassModule(int module) {
	Module::moduleInfo modInfo = _modules[module]->getModuleInfo();

	//checks if the four neightbourds the module has, carries water
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


	_modules[module]->changeModuleInfo().result = receivesWater;
	//std::cout << "Module " << module << " checking if it receives water: " << receivesWater << std::endl;


	
}


void PipePuzzleScene::waterPassPath(int path) {

	//checks if the condition needed for a path to have water is correct
	//it checks that the element is conected to carries water and if its s module, is in the right direction
	if (_waterPath[path]._whoTocheck.name == 'M') {
		auto moduleInfo = _modules[_waterPath[path]._whoTocheck.num]->getModuleInfo();
		if (moduleInfo.result && moduleInfo.dir == _waterPath[path]._whoTocheck.dir) {
			_waterPath[path]._withWater = true; 
		}
		else {
			_waterPath[path]._withWater = false;
		}
	}
	else if (_waterPath[path]._whoTocheck.name == 'P') 
	{
		_waterPath[path]._withWater = _waterPipes[_waterPath[path]._whoTocheck.num]->getPipeInfo().result;
	}

	
	if (_waterPath[path]._withWater) {
		// texture with water

		for (int i=0;i<_waterPath[path]._pathPieces.size();i++)
		{
			Image* img = _waterPath[path]._pathPieces[i].first->getMngr()->getComponent<Image>(_waterPath[path]._pathPieces[i].first);
			if (_waterPath[path]._pathPieces[i].second == 1)//L pipe 
			{
				img->setTexture(&sdlutils().images().at("pathWith1"));
			}
			else //straight
			{
				img->setTexture(&sdlutils().images().at("pathWith2"));
			}
		}
	
	}
	else {
		for (int i = 0; i < _waterPath[path]._pathPieces.size(); i++)
		{
			Image* img = _waterPath[path]._pathPieces[i].first->getMngr()->getComponent<Image>(_waterPath[path]._pathPieces[i].first);
			if (_waterPath[path]._pathPieces[i].second == 1)//L pipe 
			{
				img->setTexture(&sdlutils().images().at("pathWithout1"));
			}
			else //straight
			{
				img->setTexture(&sdlutils().images().at("pathWithout2"));
			}
		}
		
	}
}


void PipePuzzleScene::unload()
{
	for (auto a : _waterPipes) delete a;
	for (auto a : _modules) delete a;
}
void PipePuzzleScene::updatePuzzle() {

	//create queue for 3 types of elements that have to be updated 
	std::queue<int> pipesToUpdate, modulesToUpdate, pathsToUpdate; 

	// add modules 
	for (int i = 0; i < _modules.size(); ++i) {
		modulesToUpdate.push(i);
		//std::cout << "Module " << i << " has water." << std::endl;  
		
	}

	// add pipes
	for (int i = 0; i < _waterPipes.size(); ++i) {
		pipesToUpdate.push(i);
		//std::cout << "Pipe " << i << " has water." << std::endl;  
		
	}

	// add paths
	for (int i = 0; i < _waterPath.size(); ++i) {
		pathsToUpdate.push(i);
		//std::cout << "Path " << i << " has water." << std::endl;  
	}

	//while there is something to update we process
	while (!pipesToUpdate.empty() || !modulesToUpdate.empty() || !pathsToUpdate.empty()) {

		//Update modules
		while (!modulesToUpdate.empty()) {

			//get the index of the fist one
			int moduleIndex = modulesToUpdate.front();

			//pop to have the next one ready
			modulesToUpdate.pop();
			//std::cout << "Updating Module " << moduleIndex << std::endl;  

			// update x module
			waterPassModule(moduleIndex);

			// if module has water, neightbour pipes need to be checked
			if (_modules[moduleIndex]->getModuleInfo().result) {
				for (int i = 0; i < _waterPipes.size(); ++i) {
					if (_waterPipes[i]->isConnectedToModule(moduleIndex) && !_waterPipes[i]->getPipeInfo().result) {

						pipesToUpdate.push(i);  // x pipe needs to be checkeds
						//std::cout << "Pipe " << i << " is now receiving water." << std::endl;
					}
				}
			}
		}

		//Update pipes
		while (!pipesToUpdate.empty()) {

			//get the index of the fist one
			int pipeIndex = pipesToUpdate.front();

			//pop to have the next one ready
			pipesToUpdate.pop();
			//std::cout << "Updating Pipe " << pipeIndex << std::endl; 

			// update x pipe
			waterPassPipe(pipeIndex);

			// if pipe has water, we need to check the paths
			if (_waterPipes[pipeIndex]->getPipeInfo().result) {
				for (int i = 0; i < _waterPath.size(); ++i) {
					// check that pipe and path are conected anf path doesnt have water for it to be updated
					if (_waterPath[i]._whoTocheck.num == pipeIndex && !_waterPath[i]._withWater) {
						pathsToUpdate.push(i);  // x path needs to be checked
						//std::cout << "Path " << i << " is now receiving water." << std::endl;  
					}
				}
			}
		}

		//Update paths
		while (!pathsToUpdate.empty()) {

			//get the index of the fist one
			int pathIndex = pathsToUpdate.front();

			//pop to have the next one ready
			pathsToUpdate.pop();
			//std::cout << "Updating Path " << pathIndex << std::endl;

			// safe the previous water state
			bool previousWaterState = _waterPath[pathIndex]._withWater;

			// update x path
			waterPassPath(pathIndex); 

			// ckeck if path has change water state
			bool waterChanged = (_waterPath[pathIndex]._withWater != previousWaterState);

			// we only readd it if state of water has changed
			if (_waterPath[pathIndex]._withWater && waterChanged) {
				pathsToUpdate.push(pathIndex);  // if now the path has water we added again for it to be checked
				
			}
		}

	}

	// Verificar si el puzzle está resuelto
	Check();
}

void PipePuzzleScene::Win()
{
	//puts the gloves in scene
	// create entity
	auto gloveEntity = entityManager->addEntity();

	// add transfomr
	auto cubeTransform = entityManager->addComponent<Transform>(
		gloveEntity, Vector2D(900, 580), Vector2D(0, 0), 150, 150, 0
	);
	// add image
	entityManager->addComponent<Image>(gloveEntity, &sdlutils().images().at("gloves"));

	// add area of visualization of the image
	entityManager->addComponent<RectArea2D>(gloveEntity);

	//add click component
	ClickComponent* clk = entityManager->addComponent<ClickComponent>(gloveEntity);
	clk->connect(ClickComponent::JUST_CLICKED, [this]() {
		//add object to inventory and deeactivates visibility of the image;
		});

	//gloveEntity->getMngr()->setActive(gloveEntity, false);

}

void PipePuzzleScene::addToInventory()
{

}

void PipePuzzleScene::Exit()
{
	Game::Instance()->getSceneManager()->popScene();
}



PipePuzzleScene::~PipePuzzleScene()
{
	unload();
}
