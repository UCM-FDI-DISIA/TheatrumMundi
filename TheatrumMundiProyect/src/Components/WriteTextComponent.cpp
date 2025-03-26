#include "WriteTextComponent.h"

#include "../../src/sdlutils/Font.h"
#include "../../src/sdlutils/Texture.h"

#include "../../TheatrumMundi/TextInfo.h"
#include "../../src/Components/Image.h"
#include "../../src/Components/Transform.h"
#include "../ecs/Manager.h"

#include <list>
#include <string>

using namespace std;

//CONSTRUCTOR
template <typename T>
WriteTextComponent<T>::WriteTextComponent(Font& desiredFont, const SDL_Color& desiredColor, T* text)
	:_myFont(desiredFont), _color(desiredColor), textStructure(text), _currentText(" "), charsToShow(0)
{

}

//UPDATE
template <>
void WriteTextComponent<std::list<TextInfo>>::update()
{

}

template <typename T>
void WriteTextComponent<T>::update()
{

}

template <>
void WriteTextComponent<TextInfo>::update()
{
	int currentTime = sdlutils().virtualTimer().currRealTime(); //get real time
	if (currentTime - lastUpdate >= TEXT_SPEED) //update currentText
	{
		if (charsToShow < textStructure->Text.size()) //there is more textLine to animate
		{
			charsToShow++;
			lastUpdate = currentTime;
		}
	}
	_currentText = textStructure->Text.substr(0, charsToShow); //substr = returns a minor string with initial pos in 0 and end in charsToShow
}


//RENDER
template <>
void WriteTextComponent<std::list<TextInfo>>::render()
{
	if (textStructure->empty()) return;
	// Definir el tamaño total de la textura final
	int totalWidth = 1000; // Ajusta según sea necesario
	int totalHeight = 0;  // Se calculará dinámicamente

	// Calcular la altura total sumando las alturas de cada elemento
	for (const auto& it : *textStructure)
	{
		if (it.Character == "/") totalHeight += 100;
		else totalHeight += 150;
	}

	_textTransform->setWidth(totalWidth);
	_textTransform->setHeight(totalHeight);

	// Crear la textura final con el tamaño adecuado
	SDL_Texture* sdlFinalTexture = SDL_CreateTexture(
		sdlutils().renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, totalWidth, totalHeight);

	// Habilitar mezcla alfa en la textura final
	SDL_SetTextureBlendMode(sdlFinalTexture, SDL_BLENDMODE_BLEND); //Habilita transparencia

	// Guardar el render target actual
	SDL_Texture* prevTarget = SDL_GetRenderTarget(sdlutils().renderer());

	// Establecer la nueva textura como el render target
	SDL_SetRenderTarget(sdlutils().renderer(), sdlFinalTexture);

	// Limpiar la textura con transparencia (alpha = 0)
	SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 0, 0); //Fondo completamente transparente
	SDL_RenderClear(sdlutils().renderer());

	// Renderizar cada elemento en la textura final
	int y = 50;
	for (const auto& it : *textStructure)
	{
		if (it.Character == "/")
		{
			// Línea divisoria
			Texture divideLine(sdlutils().renderer(), "...--.-.-.-.-.--.-.-.-.-.-..-.--.-.-.-.---......-----...-----.....----....---...---..-.-.-.-.-.-.-.-.-.-.", _myFont, _color);
			SDL_Rect dstVRect = { 350, y, divideLine.width(), divideLine.height() };
			divideLine.render(dstVRect, 0.0);
			y += 100;
		}
		else
		{
			// Autor
			Texture authorTexture(sdlutils().renderer(), it.Character, _myFont, _color);
			SDL_Rect dstAuthorRect = { 400, y, authorTexture.width(), authorTexture.height() };
			authorTexture.render(dstAuthorRect, 0.0);

			// Texto
			Texture textTexture(sdlutils().renderer(), it.Text, _myFont, _color);
			SDL_Rect dstRect = { 400, y + 50, textTexture.width(), textTexture.height() };
			textTexture.render(dstRect, 0.0);

			y += 150;
		}
	}

	// Restaurar el render target original
	SDL_SetRenderTarget(sdlutils().renderer(), prevTarget);

	// Convertir SDL_Texture* en Texture y asegurarse de que respete la transparencia
	Texture* finalText = new Texture(sdlutils().renderer(), sdlFinalTexture);
	_imageTextLog->setTexture(finalText);
	


	/*
	//TITLE
	Texture* titleText = new Texture(sdlutils().renderer(), "LOG",
		sdlutils().fonts().at("TITLE"), _color); //convert text to texture
	SDL_Rect dstTRect = { 500, 10, titleText->width(), titleText->height() }; //destiny rect
	titleText->render(dstTRect, 0.0); //render


	//RENDER LOG LIST
	int y = 100;
	for (const auto& it : *textStructure)
	{
		if (it.first == "/") //end of dialogue event
		{
			//Insert divide line (temporarly a string)
			Texture* divideLine = new Texture(sdlutils().renderer(), "...--.-.-.-.-.--.-.-.-.-.-..-.--.-.-.-.---......-----...-----.....----....---...---..-.-.-.-.-.-.-.-.-.-.",
				_myFont, _color); //convert text to texture
			SDL_Rect dstVRect = { 10, y, divideLine->width(), divideLine->height() }; //destiny rect
			divideLine->render(dstVRect, 0.0); //render
			y += 100;
		}
		else
		{
			//author
			Texture* authorTexture = new Texture(sdlutils().renderer(), it.first, _myFont, _color); //convert text to texture
			SDL_Rect dstAuthorRect = { 500, y, authorTexture->width(), authorTexture->height() }; //destiny rect
			authorTexture->render(dstAuthorRect, 0.0); //render

			//text
			Texture* textTexture = new Texture(sdlutils().renderer(), it.second, _myFont, _color); //convert text to texture
			SDL_Rect dstRect = { 500, y + 100, textTexture->width(), textTexture->height() }; //destiny rect
			textTexture->render(dstRect, 0.0); //render
			y += 200;
		}

	}
	*/
	
}

template<typename T>
void WriteTextComponent<T>::initComponent()
{
	auto mngr = _ent->getMngr();
	_textTransform = mngr->getComponent<Transform>(_ent);
	_imageTextLog = mngr->getComponent<Image>(_ent);
}

template <>
void WriteTextComponent<TextInfo>::render()
{
	if (_currentText.empty()) return;

	Texture* nameText = new Texture(sdlutils().renderer(), textStructure->Character, _myFont, _color);
	SDL_Rect nameRect = { 350, 465,nameText->width(),nameText->height()};
	nameText->render(nameRect, 0);

	Texture* dialogText = new Texture(sdlutils().renderer(), _currentText, _myFont, _color);
	SDL_Rect dialogRect = { 350, 550,dialogText->width(),dialogText->height() };
	dialogText->render(dialogRect, 0);
	
}

template <>
void WriteTextComponent<string>::render()
{
	if (textStructure->empty()) return;

	Texture* dialogText = new Texture(sdlutils().renderer(), *textStructure, _myFont, _color);
	SDL_Rect dialogRect = { 350, 550,dialogText->width(),dialogText->height() };
	dialogText->render(dialogRect, 0);

}

//IS FINISHED
template<>
bool WriteTextComponent<std::list<TextInfo>>::isFinished()
{
	return false;
}

template<>
bool WriteTextComponent<TextInfo>::isFinished()
{
	return charsToShow >= textStructure->Text.size() || textStructure->Text == " ";
}


//FINISH TEXTLINE
template<>
void WriteTextComponent<std::list<TextInfo>>::finishTextLine()
{
}

template<>
void WriteTextComponent<TextInfo>::finishTextLine()
{
	charsToShow = textStructure->Text.size();
}


//START TEXTLINE
template<>
void WriteTextComponent<std::list<TextInfo>>::startTextLine()
{
}

template<>
void WriteTextComponent<TextInfo>::startTextLine()
{
	charsToShow = 0;
}


//DESTROYER
template <typename T>
WriteTextComponent<T>::~WriteTextComponent()
{
}


// Explicit instantiation declaration
template class WriteTextComponent<TextInfo>;

template class WriteTextComponent<std::list<TextInfo>>;

template class WriteTextComponent<string>;
