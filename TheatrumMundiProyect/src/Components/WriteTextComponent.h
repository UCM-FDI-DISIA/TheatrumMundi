#pragma once

#include <string>

#include "../sdlutils/SDLUtils.h"

#include "../ecs/Component.h"

class TextInfo;
class Image;
class Transform;

template <typename T>
class WriteTextComponent : public ecs::Component
{
private:
	Font& _myFont;
	SDL_Color _color; // White
	T* textStructure; //points to text structure. Log list or TextInfo.
	// If it is log list, there is no Dynamic memory to destroy
	// If it is TextInfo, DialogueManager already destroys that memory.

	Image* _imageTextLog;
	Transform* _textTransform;

	const int TEXT_SPEED = 200; //20 ms
	int lastUpdate;
	std::string _currentText;
	int charsToShow;

public:
	//ID
	__CMPID_DECL__(ecs::cmp::WRITE_TEXT_COMPONENT);

	WriteTextComponent(Font& desiredFont, const SDL_Color& desiredColor, T* text);


	void update() override;
	void render() override;
	void initComponent() override;


	bool isFinished(); //checks if current TextLine has been displayed entirely on screen
	void finishTextLine(); //finishes display of current TextLine
	void startTextLine(); //resets values to start displaying new TextLine

	~WriteTextComponent();
	
};


