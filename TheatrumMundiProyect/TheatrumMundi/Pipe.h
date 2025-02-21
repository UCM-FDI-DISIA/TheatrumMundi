#pragma once
#include <vector>
#include "Direction.h"
using namespace std;
class Pipe :public Direction
{
protected:
	enum Type {

		TWO,//2 entries need to be true
		ONE// 1 entrie needs to be true
	};

	struct Entry {
		char id;
		int value;
		Direction direction;
	};

	struct pipeInfo
	{
		Type type;
		Entry entry1;
		Entry entry2;
		bool result;
	};

private:

	Type _type;
	pipeInfo _info;
public:
	Pipe(pipeInfo info, Type type);
	~Pipe();
	pipeInfo getPipeInfo() const;
	pipeInfo& changePipeInfo();
};



