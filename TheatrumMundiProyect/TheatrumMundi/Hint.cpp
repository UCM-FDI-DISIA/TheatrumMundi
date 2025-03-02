#include "Hint.h"
#include "Texture.h"

Hint::Hint(const std::string& _id, const std::string& _desc, Texture* _img)
	: id(_id), description(_desc), img(_img), active(true), x(100), y(100)
{

}

Hint::Hint()
	: id(""), description(""), img(nullptr)
{
}

Hint::~Hint()
{

}

void Hint::render(int x, int y) const
{
	std::cout << "AAAAA" << std::endl;
	SDL_Rect dest = { x, y, 200, 200 };
	img->render(dest);
	std::cout << "CONSEGUIDO" << std::endl;
}
