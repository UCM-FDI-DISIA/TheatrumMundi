#include "Pipe.h"

Pipe::Pipe(pipeInfo info): _type(info.type), _info(info)
{

}
Pipe::~Pipe()
{
}
Pipe::pipeInfo Pipe::getPipeInfo() const
{ 
	return _info; 
}

Pipe::pipeInfo& Pipe::changePipeInfo()
{
	return _info;
}

bool Pipe::isConnectedToModule(int  moduleIndex)
{
	return (_info.entry1.name == 'M' && _info.entry1.num == moduleIndex) ||
		(_info.entry2.name == 'M' && _info.entry2.num == moduleIndex);
}


void Pipe::resetPipe()
{
	_info.result = false;  
	

}
