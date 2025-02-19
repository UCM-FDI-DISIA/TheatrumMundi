#pragma once
#include <vector>
using namespace std;
class Module 
{


	
protected:
	enum Directions { // diections a module can have
		DOWN,
		UP,
		RIGHT,
		LEFT
	};

	struct moduleInfo
	{
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


