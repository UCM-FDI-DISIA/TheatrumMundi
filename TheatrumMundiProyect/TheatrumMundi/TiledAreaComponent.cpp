#include "TiledAreaComponent.h"
#include <SDL.h>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "CircleArea2D.h"
#include "RectArea2D.h"
#include "../utils/Collisions.h"
TiledAreaComponent::TiledAreaComponent(Area2DLayerManager* areaLayerMngr, Transform* objTransform, int width, int height, int fil, int col)
:Area2D(areaLayerMngr,_objTransform->getPos()){
    _areaLayerMngr = areaLayerMngr;
    objTransform = _objTransform;
    _width = width;
    _height = height;

    matcol = std::vector(fil, std::vector(col, true));
    matcopos = std::vector(fil, std::vector(col, Vector2D(0,0)));
    //init the relative pos respect the object 0,0
    unitw = _width / fil;
    unith = _height / col;
   
    for (int i = 0; i < fil;i++) {
        for (int j = 0; j< col;j++) {
            matcopos[i][j].setX(i*unitw);
            matcopos[i][j].setY(j*unith);
        }
    }
}



TiledAreaComponent::~TiledAreaComponent()
{
}

bool TiledAreaComponent::containsPoint(Vector2D point)
{

    int wordX = _objTransform->getPos().getX() + _localPosition.getX();
    int wordY = _objTransform->getPos().getY() + _localPosition.getY();

    SDL_Rect rect = {
    wordX,
    wordY,
    _width,
    _height
    };

    SDL_Point pnt = { point.getX(), point.getY() };

    if (SDL_PointInRect(&pnt, &rect)) {
        for (int i = 0; i < matcol.size();i++) {
            for (int j = 0; j < matcol[i].size(); j++)
            {
                SDL_Rect auxrect = {
                wordX+(i*unitw),
                wordY+ (i * unith),
                unitw,
                unith
                };
                if (SDL_PointInRect(&pnt, &auxrect)) {
                    return matcol[i][j];
                }
            }
        }
    }
    return false;
}

bool TiledAreaComponent::overlapsWithArea(Area2D* area)
{
    return area->_overlapsWith(this);
}

bool TiledAreaComponent::_overlapsWith(RectArea2D* rectArea)
{
    Transform* extrentTr = rectArea->getContext()->getMngr()->getComponent<Transform>(rectArea->getContext());

    if (_objTransform == nullptr || extrentTr == nullptr) return false;

    SDL_Rect thisRect = {
        _objTransform->getPos().getX() + _localPosition.getX(),

        _objTransform->getPos().getY() + _localPosition.getY(),
        _width,
        _height
    };

    SDL_Rect otherRect = {
        extrentTr->getPos().getX() + rectArea->getLocalPos().getX(),
        extrentTr->getPos().getY() + rectArea->getLocalPos().getY(),
        rectArea->getWidth(),
        rectArea->getHeight()
    };

    SDL_Rect rs;
    if (SDL_IntersectRect(&thisRect, &otherRect, &rs)) return this->CheckCollisionInTiles(rs);
    else
    {
        return false;
    }
}

bool TiledAreaComponent::_overlapsWith(TiledAreaComponent* area)
{
    
    Transform* extrentTr = area->getContext()->getMngr()->getComponent<Transform>(area->getContext());

    if (_objTransform == nullptr || extrentTr == nullptr) return false;

    SDL_Rect thisRect = {
        _objTransform->getPos().getX() + _localPosition.getX(),

        _objTransform->getPos().getY() + _localPosition.getY(),
        _width,
        _height
    };

    SDL_Rect otherRect = {
        extrentTr->getPos().getX() + area->getLocalPos().getX(),
        extrentTr->getPos().getY() + area->getLocalPos().getY(),
        area->getWidth(),
        area->getHeight()
    };

    SDL_Rect rs;
    if (SDL_IntersectRect(&thisRect, &otherRect, &rs)) return (area->CheckCollisionInTiles(rs) && this->CheckCollisionInTiles(rs));
    else
    {
        return false;
    }
}


bool TiledAreaComponent::_overlapsWith(CircleArea2D* rectArea)
{
    return false;
}

void TiledAreaComponent::setActiveTile()
{
}

bool TiledAreaComponent::CheckCollisionInTiles(SDL_Rect& _collition)
{
    _collition.x -= _objTransform->getPos().getX();
    _collition.y -= _objTransform->getPos().getY();
    Vector2D auxpos;
    for (int i = 0; i < matcol.size();i++) {
        for (int j = 0; j < matcol[i].size(); j++)
        {
            auxpos = matcopos [i][j];
            if (_collition.x>=auxpos.getX()&& _collition.x <= auxpos.getX()+unitw&&
                _collition.y >= auxpos.getY() && _collition.x <= auxpos.getY() + unith) {
                return matcol[i][j];
            }
        }
    }
    return false;
}
