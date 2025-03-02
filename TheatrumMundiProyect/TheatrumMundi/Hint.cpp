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
	if (active && img)
	{
		SDL_Rect dest = { x, y, img->width(), img->height() };
		img->render(dest);
		std::cout << "Rendering hint. Active: " << active << std::endl;
	}
}
