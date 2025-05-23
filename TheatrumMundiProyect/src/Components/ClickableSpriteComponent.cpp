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
    _transform = nullptr;
    _tr = nullptr;
    _img = nullptr;
    _transformOriginal = nullptr;
}

ClickableSpriteComponent::~ClickableSpriteComponent()
{
}

void ClickableSpriteComponent::initComponent() {
    if (_ent != nullptr) {
        _transform = _ent->getMngr()->getComponent<Transform>(_ent);
        _img = _ent->getMngr()->getComponent<Image>(_ent);
        _transformOriginal = _ent->getMngr()->getComponent<Transform>(_ent);
        _tr = _ent->getMngr()->getComponent<TriggerComponent>(_ent);
        alphaCursorEntered = 200;
        alphaCursorExit = 255;

        if (_img != nullptr && _transform != nullptr && _transformOriginal != nullptr && _tr != nullptr) {
            _tr->connect(TriggerComponent::CURSOR_ENTERED, [this]() {
                _img->setW(_transform->getWidth() - 10);
                _img->setH(_transform->getHeight() - 10);
                _img->addPosOffset(5, 5);
                //_transform->getPos().set(_transform->getPos() + Vector2D(5,5));
                });

            _tr->connect(TriggerComponent::CURSOR_LEFT, [this]() {
                _img->setW(_transform->getWidth());
                _img->setH(_transform->getHeight());
                _img->addPosOffset(-5, -5);
                //_transform->getPos().set(_transform->getPos() + Vector2D(-5,-5));
                });
        }

    }
        //_tr->connect(TriggerComponent::CURSOR_ENTERED, [this]() {
        //    _ent->getMngr()->getComponent<Image>(_ent)->setTexture(&sdlutils().images().at(_idImage + "Selected"));
        //});
        //
        //_tr->connect(TriggerComponent::CURSOR_LEFT, [this]() {
        //    _ent->getMngr()->getComponent<Image>(_ent)->setTexture(&sdlutils().images().at(_idImage));
        //});
}