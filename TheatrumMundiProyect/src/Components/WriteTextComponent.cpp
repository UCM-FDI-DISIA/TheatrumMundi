
#include "WriteTextComponent.h"

#include "../../src/sdlutils/Font.h"
#include "../../src/sdlutils/SDLUtils.h"
#include "../../src/sdlutils/Texture.h"



WriteTextComponent::WriteTextComponent()
{
}

void WriteTextComponent::ShowDialogue(TextInfo dialog) 
{
	actualLine = dialog;

}

void WriteTextComponent::update()
{
}

void WriteTextComponent::render()
{
	//actualLine = { "hola","me cago en mi puta vida" };
	//std::cout << actualLine.Character << " " << actualLine.Text << std::endl;
	Font& fuente = sdlutils().fonts().at("BASE");
	SDL_Color rojo = { 255, 0, 0, 255 };

	Texture* nameText = new Texture(sdlutils().renderer(), actualLine.Character, fuente, rojo);
	SDL_Rect nameRect = { 500, 0,nameText->width(),nameText->height()};
	nameText->render(nameRect, 0);

	Texture* dialogText = new Texture(sdlutils().renderer(), actualLine.Text, fuente, rojo);
	SDL_Rect dialogRect = { 500, 100,dialogText->width(),dialogText->height() };
	dialogText->render(dialogRect, 0);
	


}


WriteTextComponent::~WriteTextComponent()
{
}
