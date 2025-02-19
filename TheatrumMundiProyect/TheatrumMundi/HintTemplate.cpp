#include "HintTemplate.h"

HintTemplate::HintTemplate() : description(""), image(nullptr)
{

}

HintTemplate::HintTemplate(const string& _desc, Texture* _tex) : description(_desc), image(new Image(_tex))
{

}

HintTemplate::~HintTemplate()
{
	delete image;
}
