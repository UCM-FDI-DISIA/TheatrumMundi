#include "WriteTextComponent.h"

#include "../../src/sdlutils/Font.h"
#include "../../src/sdlutils/Texture.h"


#include <list>


template <typename T>
WriteTextComponent<T>::WriteTextComponent(Font& desiredFont, SDL_Color desiredColor, T* text)
	:_myFont(desiredFont), _color(desiredColor), textStructure(text)
{
}


template <typename T>
void WriteTextComponent<T>::ShowDialogue(T* dialog)
{
	textStructure = dialog;

}

template <typename T>
void WriteTextComponent<T>::update()
{
}

template <>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::render()
{

	int y = 0;
	for (auto it = textStructure->begin(); it != textStructure->end(); ++it)
	{
		//author
		Texture* authorTexture = new Texture(sdlutils().renderer(), it->first, _myFont, _color); //convert text to texture
		SDL_Rect dstARect = { 500, y, authorTexture->width(), authorTexture->height() }; //destiny rect
		authorTexture->render(dstARect, 0.0); //render

		//text
		Texture* textTexture = new Texture(sdlutils().renderer(), it->second, _myFont, _color); //convert text to texture
		SDL_Rect dstRect = { 500, y + 100, textTexture->width(), textTexture->height() }; //destiny rect
		textTexture->render(dstRect, 0.0); //render

		y += 200;
	}
}

template <>
void WriteTextComponent<TextInfo>::render()
{
	//actualLine = { "hola","me cago en mi puta vida" };
	//std::cout << actualLine.Character << " " << actualLine.Text << std::endl;
	Font& fuente = sdlutils().fonts().at("BASE");
	SDL_Color rojo = { 255, 0, 0, 255 };

	Texture* nameText = new Texture(sdlutils().renderer(), textStructure->Character, fuente, rojo);
	SDL_Rect nameRect = { 500, 0,nameText->width(),nameText->height()};
	nameText->render(nameRect, 0);

	Texture* dialogText = new Texture(sdlutils().renderer(), textStructure->Text, fuente, rojo);
	SDL_Rect dialogRect = { 500, 100,dialogText->width(),dialogText->height() };
	dialogText->render(dialogRect, 0);
	
}



template <typename T>
WriteTextComponent<T>::~WriteTextComponent()
{
}

// Declaración de instanciación explícita
template class WriteTextComponent<TextInfo>;

template class WriteTextComponent<std::list<std::pair<std::string,std::string>>>;
