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
