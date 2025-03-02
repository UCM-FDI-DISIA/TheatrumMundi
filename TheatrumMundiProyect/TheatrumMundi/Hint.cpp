#include "Hint.h"

Hint::Hint(const std::string& _id, const std::string& _desc, Texture* _img)
	: id(_id), description(_desc), img(_img), active(true)
{

}

Hint::~Hint()
{
	delete img;
}
