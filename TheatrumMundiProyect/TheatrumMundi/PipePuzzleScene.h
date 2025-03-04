#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include "ScenePuzzleTemplate.h"
#include "Pipe.h"
#include "Module.h"

using namespace std;
class SceneManager;
class Module;
class Pipe;
class Image;
class PipePuzzleScene : public ScenePuzzleTemplate
{

private:
	
	//name of the neightbour elem
	// num is the id of the element
	//direction: dir neightbour has (for modules it will be a dir, for pipes will be none )
	struct Type {
		char name;
		int num;
		Directions dir;
		
	};

	//id: where in the array it is
	//_withWater: if it has water
	//_whoTocheck: which neighbour needs to have water in order for the path to have water
	// _pathPieces: sub vector of pieces a path has
	struct waterPath {

		int id;
		bool _withWater;
		Type _whoTocheck;
		std::vector<pair<ecs::Entity*,int>>_pathPieces; //if path has l form its 1 if its straight 2
		
	};
	
	std::vector<waterPath>_waterPath; //vector that contains the amount of paths 
	std::vector<Pipe*> _waterPipes; //vector that contains the amount of water each pipe has
	std::vector<Module*> _modules;//modules that change the direction of the water flow
	bool solved;
	std::vector<ecs::Entity*>_modulesEnt;
	std::vector<ecs::Entity*>_pipesEnt;
	std::vector<ecs::Entity*>_pathEnt;

protected:


public: 
	 PipePuzzleScene();
	 void pipeCreation();
	 void moduleCreation();
	 void pathCreation();
	 bool Check() override; 
	 void changeDirection(int module); //changes direction of the module
	 void init() override;
	 void waterPassPipe(int pipe); //if water can pass through a pipe (if it recives water from its neightbours)
	 void waterPassModule(int module); //if water can pass through a module (if it recives water from its neightbours)
	 void waterPassPath(int path);// series of conditions to check if a path has water
	 void unload() override;
	 void updatePuzzle() ;
	 void Win() override;
	 void addToInventory();
	 ~PipePuzzleScene();
};

