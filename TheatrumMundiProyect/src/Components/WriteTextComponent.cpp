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

	// Scale
	float scaleX = Game::Instance()->wscreenScale;
	float scaleY = Game::Instance()->hscreenScale;

	int y = 50 * scaleY;

	for (const auto& it : *textStructure)
	{
		if (it.Character == "/")
		{
			Texture* divideLine = new Texture(sdlutils().renderer(),
				"...--.-.-.-.-.--.-.-.-.-.-..-.--.-.-.-.---......-----...-----.....----....---...---..-.-.-.-.-.-.-.-.-.-.",
				_myFont, _color);

			SDL_Rect dstVRect = { static_cast<int>(350 * scaleX), y,
				divideLine->width(), divideLine->height() };

			divideLine->render(dstVRect, 0.0);
			y += 80 * scaleY;
			delete divideLine;

		}
		else
		{
			// Author
			Texture* authorTexture = new Texture(sdlutils().renderer(), it.Character, _myFont, _color);
			SDL_Rect dstAuthorRect = {
				static_cast<int>(400 * scaleX),
				y,
				authorTexture->width(), authorTexture->height()
			};
			authorTexture->render(dstAuthorRect, 0.0);

			if (it.Character == " ") y += static_cast<int>(25 * scaleY);
			else y += static_cast<int>(50 * scaleY);

			// Split text
			std::vector<std::string> lines = splitTextByNewline(it.Text);
			int currentY = y;

			for (const auto& line : lines)
			{
				Texture* textTexture = new Texture(sdlutils().renderer(), line, _myFont, _color);
				SDL_Rect dstRect = {
					static_cast<int>(400 * scaleX),
					currentY,
					textTexture->width(), textTexture->height()
				};
				textTexture->render(dstRect, 0.0);
				currentY += textTexture->height() + 5;
				delete textTexture;
				
			}

			y += static_cast<int>(100 * scaleY); // Space between split lines
			delete authorTexture;
		}
	}

	
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
			delete dialogText;
		}
	}
	else
	{
		// Text
		std::vector<std::string> lines = splitTextByNewline(_currentText); //splits text into different lines

		int y = 550* Game::Instance()->hscreenScale;  // initial dialogue text

		for (const auto& line : lines) {
			Texture* dialogText =  new Texture(sdlutils().renderer(), line, _myFont, _color);
			SDL_Rect dialogRect = { 375* Game::Instance()->wscreenScale, y, dialogText->width()* Game::Instance()->wscreenScale, dialogText->height()* Game::Instance()->hscreenScale };
			dialogText->render(dialogRect, 0);

			y += dialogText->height() + 5* Game::Instance()->hscreenScale;  // space between split lines
			delete dialogText;
		
		}
	}
	delete nameText;
	
	
}

template <>
void WriteTextComponent<DescriptionInfo>::render()
{
	if (textStructure->Description.empty()) return;

	std::vector<std::string> lines = splitTextByNewline(textStructure->Description); //splits text into different lines

	int y = (textStructure->posY + 60) * Game::Instance()->hscreenScale;
	int x = 550 * Game::Instance()->wscreenScale;

	// scale background
	int totalHeight = 0;
	int maxWidth = 0;

	// calculate background scale
	std::vector<Texture*> renderedLines;
	for (const auto& line : lines) {
		auto dialogText = new Texture(sdlutils().renderer(), line, _myFont, _color);
		renderedLines.push_back(dialogText);

		totalHeight += dialogText->height() + (5 * Game::Instance()->hscreenScale); // split lines space
		maxWidth = std::max(maxWidth, dialogText->width());
	}

	// Background
	Texture* backgroundText = &sdlutils().images().at("fondoPruebaLog");
	SDL_Rect backgroundTextRect = {(x - 10), (textStructure->posY + 50) * Game::Instance()->hscreenScale,(maxWidth + 50) * Game::Instance()->wscreenScale,(totalHeight + 25) * Game::Instance()->hscreenScale};
	backgroundText->render(backgroundTextRect, 0);

	// Render description
	for (auto* dialogText : renderedLines)
	{
		SDL_Rect dialogRect = {	x, y, dialogText->width() * Game::Instance()->wscreenScale,	dialogText->height() * Game::Instance()->hscreenScale};
		dialogText->render(dialogRect, 0);
		y += dialogText->height() + 5 * Game::Instance()->hscreenScale; 
		delete dialogText;
	}

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
