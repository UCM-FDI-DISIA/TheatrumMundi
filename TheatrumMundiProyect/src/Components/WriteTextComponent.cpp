#include "WriteTextComponent.h"

#include "../../src/sdlutils/Font.h"
#include "../../src/sdlutils/Texture.h"

#include "../../TheatrumMundi/TextInfo.h"
#include "../../TheatrumMundi/DescriptionInfo.h"
#include "../../src/Components/Image.h"
#include "../../src/Components/Transform.h"
#include "../ecs/Manager.h"
#include "Game.h"
#include <list>
#include <string>
#include <sstream>

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

/// <summary>
/// Divides a string with \n
/// </summary>
/// <param name="text">string to divide</param>
/// <returns>vector of divided dialogue lines</returns>
vector<string> splitTextByNewline(const string& text) {
	vector<string> dividedLines; //vector to return
	stringstream ss(text); //stringstream detects \n
	string line; //temporal line read with stringstream

	// Reads all lines of text
	while (getline(ss, line))
	{
		dividedLines.push_back(line);
	}

	return dividedLines;
}

//RENDER
template <>
void WriteTextComponent<std::list<TextInfo>>::render()
{
	if (textStructure->empty()) return;
	// Definir el tama�o total de la textura final
	int totalWidth = 1000*Game::Instance()->wscreenScale; // Ajusta seg�n sea necesario
	int totalHeight = 800*Game::Instance()->hscreenScale;  // Se calcular� din�micamente

	// Crear la textura final con el tama�o adecuado
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
			// L�nea divisoria
			Texture divideLine(sdlutils().renderer(), "...--.-.-.-.-.--.-.-.-.-.-..-.--.-.-.-.---......-----...-----.....----....---...---..-.-.-.-.-.-.-.-.-.-.", _myFont, _color);
			SDL_Rect dstVRect = { 350, y,
				divideLine.width(), divideLine.height() };
			divideLine.render(dstVRect, 0.0);
			y += 80;
		}
		else
		{
			// Autor
			Texture authorTexture(sdlutils().renderer(), it.Character, _myFont, _color);
			SDL_Rect dstAuthorRect = { 400, y, 
				authorTexture.width(), authorTexture.height()};
			authorTexture.render(dstAuthorRect, 0.0);

			if (it.Character == " ") { y += 25; }
			else { y += 50; }

			// Text:
			std::vector<std::string> lines = splitTextByNewline(it.Text); //splits text into different lines

			int currentY = y;

			// render each split line
			for (const auto& splitLine : lines)
			{
				Texture textTexture(sdlutils().renderer(), splitLine, _myFont, _color);
				SDL_Rect dstRect = { 400, currentY, textTexture.
					width(), textTexture.height()};
				textTexture.render(dstRect, 0.0);

				currentY += textTexture.height() + 5; // space between split lines
			}

			y += 100;
		}
	}

	// Restaurar el render target original
	SDL_SetRenderTarget(sdlutils().renderer(), prevTarget);

	// Convertir SDL_Texture* en Texture y asegurarse de que respete la transparencia
	Texture* finalText = new Texture(sdlutils().renderer(), sdlFinalTexture);
	SDL_Rect dstRect = { 0, 0, finalText->width()* Game::Instance()->wscreenScale, finalText->height()* Game::Instance()->hscreenScale };
	finalText->render(dstRect, 0.0);
}

template<typename T>
void WriteTextComponent<T>::initComponent()
{
	auto mngr = _ent->getMngr();
}

template <>
void WriteTextComponent<TextInfo>::render()
{
	if (_currentText.empty()) return;

	// Author
	Texture* nameText = new Texture(sdlutils().renderer(), textStructure->Character, _myFont, _color);
	SDL_Rect nameRect = { 325* Game::Instance()->wscreenScale, 465* Game::Instance()->hscreenScale,
		nameText->width()* Game::Instance()->wscreenScale,nameText->height()* Game::Instance()->hscreenScale };
	nameText->render(nameRect, 0);

	if (isMiddleRoom)
	{
		// Text
		std::vector<std::string> lines = splitTextByNewline(_currentText); //splits text into different lines

		int y = 550* Game::Instance()->hscreenScale;  // initial dialogue text

		for (const auto& line : lines) {
			Texture* dialogText = new Texture(sdlutils().renderer(), line, _myFont, _color);
			SDL_Rect dialogRect = { 325* Game::Instance()->wscreenScale, y,
				dialogText->width()* Game::Instance()->wscreenScale, dialogText->height()* Game::Instance()->hscreenScale };
			dialogText->render(dialogRect, 0);

			y += dialogText->height() + 5*Game::Instance()->hscreenScale;  // space between split lines
		}
	}
	else
	{
		// Text
		std::vector<std::string> lines = splitTextByNewline(_currentText); //splits text into different lines

		int y = 550* Game::Instance()->hscreenScale;  // initial dialogue text

		for (const auto& line : lines) {
			Texture* dialogText = new Texture(sdlutils().renderer(), line, _myFont, _color);
			SDL_Rect dialogRect = { 375* Game::Instance()->wscreenScale, y, dialogText->width()* Game::Instance()->wscreenScale, dialogText->height()* Game::Instance()->hscreenScale };
			dialogText->render(dialogRect, 0);

			y += dialogText->height() + 5* Game::Instance()->hscreenScale;  // space between split lines
		}
	}

	
	
}

template <>
void WriteTextComponent<DescriptionInfo>::render()
{
	if (textStructure->Description.empty()) return;

	Texture* dialogText = new Texture(sdlutils().renderer(), textStructure->Description, _myFont, _color);
	SDL_Rect dialogRect = { 680* Game::Instance()->wscreenScale, (textStructure->posY + 60)* Game::Instance()->hscreenScale,
		dialogText->width()* Game::Instance()->wscreenScale,dialogText->height()* Game::Instance()->wscreenScale };
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

template<typename T>
void WriteTextComponent<T>::setMiddleRoom(bool state)
{
	isMiddleRoom = state;
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

template class WriteTextComponent<DescriptionInfo>;
