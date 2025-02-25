#pragma once
#include <vector>
#include "Direction.h"
using namespace std;
using namespace Direction;
class Module 
{

protected:
	
	//result means if it carries water
	//dir means actual direction
	//down,right,left, up means what the module/pipe is closest to in that direction(neighbour element)

	struct moduleInfo
	{
		int name;
		Directions dir;
		pair<char, int> right;
		pair<char, int> left;
		pair<char, int> up;
		pair<char, int> down;
		bool result;
	};
private:

	moduleInfo _info;

public:
	Module(moduleInfo info);
	moduleInfo getModuleInfo() const;
	moduleInfo& changeModuleInfo() ;
	~Module();
	void resetModule();
	friend class PipePuzzleScene;
};


