#include "WriteTextComponent.h"

#include "../../src/sdlutils/Font.h"
#include "../../src/sdlutils/Texture.h"

#include "../../TheatrumMundi/TextInfo.h"

#include <list>

//CONSTRUCTOR
template <typename T>
WriteTextComponent<T>::WriteTextComponent(Font& desiredFont, const SDL_Color& desiredColor, T* text)
	:_myFont(desiredFont), _color(desiredColor), textStructure(text), _currentText(" "), charsToShow(0)
{

}

//UPDATE
template <>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::update()
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
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::render()
{
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

	
}

template <>
void WriteTextComponent<TextInfo>::render()
{

	Texture* nameText = new Texture(sdlutils().renderer(), textStructure->Character, _myFont, _color);
	SDL_Rect nameRect = { 350, 465,nameText->width(),nameText->height()};
	nameText->render(nameRect, 0);

	Texture* dialogText = new Texture(sdlutils().renderer(), _currentText, _myFont, _color);
	SDL_Rect dialogRect = { 350, 550,dialogText->width(),dialogText->height() };
	dialogText->render(dialogRect, 0);
	
}


//IS FINISHED
template<>
bool WriteTextComponent<std::list<std::pair<std::string, std::string>>>::isFinished()
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
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::finishTextLine()
{
}

template<>
void WriteTextComponent<TextInfo>::finishTextLine()
{
	charsToShow = textStructure->Text.size();
}


//START TEXTLINE
template<>
void WriteTextComponent<std::list<std::pair<std::string, std::string>>>::startTextLine()
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

template class WriteTextComponent<std::list<std::pair<std::string,std::string>>>;
