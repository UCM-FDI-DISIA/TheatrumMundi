#include "TiledAreaComponent.h"
#include "Transform.h"
#include "SDL.h"
TiledAreaComponent::TiledAreaComponent(Area2DLayerManager* areaLayerMngr, Transform* objTransform, int width, int height, int fil, int col)
:Area2D(areaLayerMngr,_objTransform->getPos()){
    _areaLayerMngr = areaLayerMngr;
    objTransform = _objTransform;
    _width = width;
    _height = height;

    matcol = std::vector(fil, std::vector(col, true));
    matcopos = std::vector(fil+1, std::vector(col+1, Vector2D(0,0)));
    //init the relative pos respect the object 0,0
    int auxunitw = _width / fil;
    int auxunith = _height / col;
    fil++;
    col++;
    for (int i = 0; i < fil;i++) {
        for (int j = 0; j< col;j++) {
            matcopos[i][j].setX(i*auxunitw);
            matcopos[i][j].setY(j*auxunith);
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

            }
        }
    }
    return false;
}

bool TiledAreaComponent::overlapsWithArea(Area2D* area)
{
    return false;
}

bool TiledAreaComponent::_overlapsWith(RectArea2D* rectArea)
{
    return false;
}

bool TiledAreaComponent::_overlapsWith(CircleArea2D* rectArea)
{
    return false;
}

void TiledAreaComponent::setActiveTile()
{
}
