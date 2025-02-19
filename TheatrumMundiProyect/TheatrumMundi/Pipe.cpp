#include "Pipe.h"

Pipe::Pipe(pipeInfo info, Type type): _type(type), _info(info)
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
