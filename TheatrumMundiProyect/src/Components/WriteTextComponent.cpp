
#include "WriteTextComponent.h"
#include "../src/sdlutils/Font.h"


WriteTextComponent::WriteTextComponent()
{
}

void WriteTextComponent::ShowDialogue(TextInfo dialog) 
{
	Font::renderText(dialog.Character);
	Font::renderText(dialog.Text);
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
