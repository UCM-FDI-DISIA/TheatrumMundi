#include "WriteTextComponent.h"

#include "../../src/sdlutils/Font.h"
#include "../../src/sdlutils/Texture.h"

#include "../../TheatrumMundi/TextInfo.h"

#include <list>


template <typename T>
WriteTextComponent<T>::WriteTextComponent(Font& desiredFont, const SDL_Color& desiredColor, T* text)
	:_myFont(desiredFont), _color(desiredColor), textStructure(text), _currentText(" "), charsToShow(0)
{

}


template <typename T>
void WriteTextComponent<T>::ShowDialogue(T* dialog)
{
	textStructure = dialog;

}

template <>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::update()
{

}

template <>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::render()
{

	int y = 0;
	for (auto it = textStructure->begin(); it != textStructure->end(); ++it)
		// for (auto it: *textStructure)
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


template <>
void WriteTextComponent<TextInfo>::render()
{

	Texture* nameText = new Texture(sdlutils().renderer(), textStructure->Character, _myFont, _color);
	SDL_Rect nameRect = { 500, 0,nameText->width(),nameText->height()};
	nameText->render(nameRect, 0);

	Texture* dialogText = new Texture(sdlutils().renderer(), _currentText.c_str(), _myFont, _color);
	SDL_Rect dialogRect = { 500, 100,dialogText->width(),dialogText->height() };
	dialogText->render(dialogRect, 0);
	
}


template<>
bool WriteTextComponent<std::list<std::pair<std::string, std::string>>>::isFinished()
{
	return false;
}

template<>
bool WriteTextComponent<TextInfo>::isFinished()
{
	return charsToShow >= textStructure->Text.size();
}

template<>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::finishTextLine()
{
}

template<>
void WriteTextComponent<TextInfo>::finishTextLine()
{
	charsToShow = textStructure->Text.size();
}

template<>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::startTextLine()
{
}

template<>
void WriteTextComponent<TextInfo>::startTextLine()
{
	charsToShow = 0;
}

template <typename T>
WriteTextComponent<T>::~WriteTextComponent()
{
}

// Explicit instantiation declaration
template class WriteTextComponent<TextInfo>;

template class WriteTextComponent<std::list<std::pair<std::string,std::string>>>;
