#pragma once

#include <string>
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"

#include "../ecs/Component.h"

class TextInfo;
class Image;
class Transform;
class DescriptionInfo;

template <typename T>
class WriteTextComponent : public ecs::Component
{
private:
	Font& _myFont;
	SDL_Color _color; // White
	T* textStructure; //points to text structure. Log list or TextInfo.
	// If it is log list, there is no Dynamic memory to destroy
	// If it is TextInfo, DialogueManager already destroys that memory.

	const int TEXT_SPEED = 35; //20 ms
	int lastUpdate;
	std::string _currentText;
	int charsToShow;
	Vector2D posOffset = Vector2D(0, 0);
	bool isMiddleRoom;

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

	void setMiddleRoom(bool state);
	void resetOffset();
	void addOffset(int x, int y);
	Vector2D getOffset();
	~WriteTextComponent();
	
};


