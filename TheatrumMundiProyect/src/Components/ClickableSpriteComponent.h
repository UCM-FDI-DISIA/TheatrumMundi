#pragma once

#include "../ecs/Component.h"

class TriggerComponent;
class TextureComponent;
class Image;
class Texture;
class Transform;

class ClickableSpriteComponent : public ecs::Component
{
public:
    __CMPID_DECL__(ecs::cmp::CLICKABLE_SPRITE_COMPONENT)

     void initComponent() override;


private:
    //TextureComponent* _tex;
    //TextureComponent* _texOriginal;
    //Image* _img;
    //Image* _imgOriginal;
    TriggerComponent* _tr;
    Transform* _transform;
    Transform* _transformOriginal;
    int alphaCursorEntered;
    int alphaCursorExit;
};