#pragma once
#include <vector>
using namespace std;
class Pipe
{
protected:
	enum Type {

		TWO,//2 entries need to be true
		ONE// 1 entrie needs to be true
	};

	struct pipeInfo
	{
		Type type;
		pair<char, int> entry1;
		pair<char, int> entry2;
		int waterAmount;
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



