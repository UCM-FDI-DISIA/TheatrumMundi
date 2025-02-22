#pragma once
#include <vector>
#include "Direction.h"
using namespace std;
class Module :public Direction
{

protected:
	
	struct moduleInfo
	{
		int id;
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
};


