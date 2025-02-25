#pragma once
#include <vector>
#include "Direction.h"
using namespace std;
using namespace Direction;
class Pipe 
{
protected:
	enum Type {

		TWO,//2 entries need to be true for water to pass through it
		ONE// 1 entrie needs to be true for water to pass through it
	};

	// name: name of the neighbour elem
	// nu: id of the neighbour
	//direction: dir neightbour has (for modules it will be a dir, for pipes will be none )
	struct Entry {
		char name;
		int num;
		Directions direction;
	};

	//result means if it carries water
	//entry1, entry 2 means what the module/pipe is closest to in that neighbour element
	struct pipeInfo
	{
		int id;
		Type type;
		Entry entry1;
		Entry entry2;
		bool result;
	};

private:

	Type _type;
	pipeInfo _info;
public:
	Pipe(pipeInfo info);
	~Pipe();
	pipeInfo getPipeInfo() const;
	pipeInfo& changePipeInfo();
	void resetPipe();
	bool isConnectedToModule(int moduleIndex);
		


	friend class PipePuzzleScene;
};



