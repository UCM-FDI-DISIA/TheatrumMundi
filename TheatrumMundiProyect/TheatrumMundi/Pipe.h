#pragma once
#include <vector>
#include "Direction.h"
using namespace std;
using namespace Direction;
class Pipe 
{
protected:
	enum Type {

		TWO,//2 entries need to be true
		ONE// 1 entrie needs to be true
	};

	struct Entry {
		char id;
		int value;
		Directions direction;
	};

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
	friend class PipePuzzleScene;
};



