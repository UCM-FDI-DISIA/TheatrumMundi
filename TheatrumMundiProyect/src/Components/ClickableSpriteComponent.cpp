#include "ClickableSpriteComponent.h"
#include "Manager.h"
#include "TriggerComponent.h"
#include "Texture.h"
#include "Image.h"
#include "Transform.h"
#include <assert.h>
#include "SDLUtils.h"

using namespace std;

ClickableSpriteComponent::ClickableSpriteComponent(std::string _idImageToPass)
{ 
    _idImage = _idImageToPass;
}

void ClickableSpriteComponent::initComponent() {
    _transform = _ent->getMngr()->getComponent<Transform>(_ent);
    _transformOriginal = _ent->getMngr()->getComponent<Transform>(_ent);
    _tr = _ent->getMngr()->getComponent<TriggerComponent>(_ent); 
    alphaCursorEntered = 200;
    alphaCursorExit = 255;

        _tr->connect(TriggerComponent::CURSOR_ENTERED, [this]() {
        _transform->setWidth(_transform->getWidth() - 10);
        _transform->setHeight(_transform->getHeight() - 10);
        _transform->getPos().set(_transform->getPos() + Vector2D(5,5));
        });
        
        _tr->connect(TriggerComponent::CURSOR_LEFT, [this]() {
        _transform->setWidth(_transform->getWidth() + 10);
        _transform->setHeight(_transform->getHeight() + 10);
        _transform->getPos().set(_transform->getPos() + Vector2D(-5,-5));
        });

        //_tr->connect(TriggerComponent::CURSOR_ENTERED, [this]() {
        //    _ent->getMngr()->getComponent<Image>(_ent)->setTexture(&sdlutils().images().at(_idImage + "Selected"));
        //});
        //
        //_tr->connect(TriggerComponent::CURSOR_LEFT, [this]() {
        //    _ent->getMngr()->getComponent<Image>(_ent)->setTexture(&sdlutils().images().at(_idImage));
        //});
}