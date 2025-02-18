
#include "WriteTextComponent.h"
#include "../src/sdlutils/Font.h"
#include "DialogueManager.h"

WriteTextComponent::WriteTextComponent() : Component()
{
}

void WriteTextComponent::ShowDialogue(TextInfo dialog)
{	
	Font fuente("BASE",12);

	SDL_Color rojo = { 255, 0, 0, 255 };

	fuente.renderText(dialog.Character,rojo);
	fuente.renderText(dialog.Text, rojo);
}

void WriteTextComponent::update()
{
}

void WriteTextComponent::render()
{
}

WriteTextComponent::~WriteTextComponent()
{
}
