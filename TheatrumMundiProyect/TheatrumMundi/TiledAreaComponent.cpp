#include "TiledAreaComponent.h"
#include <SDL.h>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "CircleArea2D.h"
#include "RectArea2D.h"
#include "../utils/Collisions.h"
using namespace std;
TiledAreaComponent::TiledAreaComponent(Area2DLayerManager* areaLayerMngr, Transform* objTransform, int fil, int col)
:Area2D(areaLayerMngr,Vector2D(0,0)){
 
    _objTransform = objTransform;
    _width = objTransform->getWidth();
    _height = objTransform->getHeight();
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
                wordY+ (j * unith),
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
        _width-10,
        _height-10  
    };

    SDL_Rect otherRect = {
        extrentTr->getPos().getX() + area->getLocalPos().getX(),
        extrentTr->getPos().getY() + area->getLocalPos().getY(),
        area->getWidth()-10,
        area->getHeight()-10
    };
   
    SDL_Rect rs;
    if (SDL_IntersectRect(&thisRect, &otherRect, &rs)) {
        bool flag = false;
        if (this->CheckCollisionInTiles(rs)) {
            area->CheckCollisionInTiles(rs);
            std::vector<SDL_Rect> othertiledcol = area->getTilesCol();
            for (SDL_Rect thidtiles : myTiledCol) {
                for (SDL_Rect othertiles : othertiledcol) {
                    //depurar los vectores de collisiones resultantes
                    if (SDL_IntersectRect(&othertiles, &thidtiles, &rs)) flag = true;
                }
            }

        }
        else return flag;
    }
    else
    {
        return false;
    }
}


bool TiledAreaComponent::_overlapsWith(CircleArea2D* circleArea)
{
    Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);
    Transform* extrentTr = circleArea->getContext()->getMngr()->getComponent<Transform>(circleArea->getContext());

    if (_objTransform == nullptr || extrentTr == nullptr) return false;
    return CheckCollisionInTilesCircles(extrentTr->getPos() + circleArea->getLocalPos(), circleArea->getRadius());
}


void TiledAreaComponent::setActiveTile(bool t, int i, int j)
{
    matcol[i][j] = t;
}

bool TiledAreaComponent::CheckCollisionInTiles(SDL_Rect& _collition)
{
    Vector2D auxpos = _objTransform->getPos();
    myTiledCol.clear();
    bool flag = false;
    for (int i = 0; i < matcol.size();i++) {
        for (int j = 0; j < matcol[i].size(); j++)
        {
            SDL_Rect auxRect = { auxpos.getX()+(i*unitw) ,auxpos.getY()+(unith*j),unitw,unith};
            SDL_Rect auxcolrect;
                if(SDL_IntersectRect(&auxRect,&_collition,&auxcolrect)){
                    if (matcol[i][j]) {
                        flag = true;
                        myTiledCol.push_back(auxcolrect);
                        std::cout << "flag " << i << j << endl;
                    }  
            }
        }
    }
    std::cout << "flag " << flag << endl;
    return flag;
}

bool TiledAreaComponent::CheckCollisionInTilesCircles(const Vector2D& pos, int rad)
{
    for (int i = 0; i < matcol.size();i++) {
        for (int j = 0; j < matcol[i].size(); j++) {
            if (Collisions::rectCollidesWithCircle(
                matcopos[i][j] + _objTransform->getPos() + _localPosition, unitw, unith,
                pos, rad
            )) return true;
        }
    }
    return false;
}


