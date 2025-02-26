#pragma once

#include "../ecs/Component.h"

#include <iostream>

class TriggerComponent;
class TextureComponent;
class Image;
class Texture;
class Transform;

class ClickableSpriteComponent : public ecs::Component
{
public:
    __CMPID_DECL__(ecs::cmp::CLICKABLE_SPRITE_COMPONENT)


    ClickableSpriteComponent(std::string _idImageToPass);
    void initComponent() override;


private:
    //Texture* _tex;
    //Texture* _texOriginal;
    //Image* _img;
    //Image* _imgOriginal;
    TriggerComponent* _tr;
    Transform* _transform;
    Transform* _transformOriginal;
    std::string _idImage;
    int alphaCursorEntered;
    int alphaCursorExit;
};