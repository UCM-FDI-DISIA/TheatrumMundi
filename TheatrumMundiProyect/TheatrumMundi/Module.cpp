#include "Module.h"

Module::Module(moduleInfo info):_info(info)
{

}
Module::moduleInfo  Module::getModuleInfo() const
{
	return _info;
}

Module::moduleInfo& Module::changeModuleInfo()
{
	return _info;
}

Module::~Module()
{
}

void Module::resetModule()
{
	
	_info.result = false;  
	
}
