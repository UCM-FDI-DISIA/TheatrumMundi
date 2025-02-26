#include "ClickableSpriteComponent.h"
#include "Manager.h"
#include "TriggerComponent.h"
#include "Texture.h"
#include "Image.h"
#include "Transform.h"
#include <assert.h>

void ClickableSpriteComponent::initComponent() {
    //_tex = _ent->getMngr()->getComponent<Texture>(_ent);
    //_texOriginal = _ent->getMngr()->getComponent<Texture>(_ent);
    //_img = _ent->getMngr()->getComponent<Image>(_ent);
    //_imgOriginal = _ent->getMngr()->getComponent<Image>(_ent);
    _transform = _ent->getMngr()->getComponent<Transform>(_ent);
    _transformOriginal = _ent->getMngr()->getComponent<Transform>(_ent);
    _tr = _ent->getMngr()->getComponent<TriggerComponent>(_ent);
    alphaCursorEntered = 200;
    alphaCursorExit = 255;

    _tr->connect(TriggerComponent::CURSOR_ENTERED, [this]() {
        //_img->setAlpha(alphaCursorEntered);
        _transform->setWidth(_transform->getWidth() - 10);
        _transform->setHeight(_transform->getHeight() - 10);
        _transform->getPos().set(_transform->getPos() + Vector2D(5,5));
        });

    _tr->connect(TriggerComponent::CURSOR_LEFT, [this]() {
        //_img->setAlpha(alphaCursorExit);
        _transform->setWidth(_transform->getWidth() + 10);
        _transform->setHeight(_transform->getHeight() + 10);
        _transform->getPos().set(_transform->getPos() + Vector2D(-5,-5));
        });
}