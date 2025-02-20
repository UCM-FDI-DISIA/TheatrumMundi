#include "Hint.h"
#include "../src/Components/Image.h"

Hint::Hint(const std::string& _id, const std::string& _desc, Image* _img)
	: id(_id), description(_desc), img(_img)
{

}

Hint::~Hint()
{
	delete img;
}
