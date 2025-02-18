#include "WriteTextComponent.h"

#include "../../TheatrumMundiProyect/src/sdlutils/macros.h"
#include "../../TheatrumMundiProyect/src/sdlutils/Texture.h"
#include "../../TheatrumMundiProyect/src/Components/Transform.h"

template <typename T>

WriteTextComponent<T>::WriteTextComponent(Font& desiredFont, SDL_Color desiredColor, T text)
	:_myFont(desiredFont), _color(desiredColor), textStructure(text)
{
}

template <typename T>
void WriteTextComponent<T>::update()
{
}


template <>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::render()
{
	
	int y = 0;
	for (auto a : textStructure)
	{
		//author
		Texture* authorTexture = new Texture(sdlutils().renderer(), a.first, _myFont, _color); //convert text to texture
		SDL_Rect dstARect = { 500, y, authorTexture->width(), authorTexture->height() }; //destiny rect
		authorTexture->render(dstARect, 0.0); //render

		//text
		Texture* textTexture = new Texture(sdlutils().renderer(), a.second, _myFont, _color); //convert text to texture
		SDL_Rect dstRect = { 500, y + 100, textTexture->width(), textTexture->height() }; //destiny rect
		textTexture->render(dstRect, 0.0); //render

		y += 200;
	}
}

template <typename T>
WriteTextComponent<T>::~WriteTextComponent()
{
}


template class WriteTextComponent<std::list<std::pair<std::string, std::string>>>;