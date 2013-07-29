#ifndef __SCENE_H
#define __SCENE_H

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include "Cone.h"

class PixelMatrix
{
public:
    uint   width , 
           height;
    float *pixels;
    void   setColor(uint u, uint v, fl3 Color);
};

struct Figure
{
    enum FIGURE_TYPE
        {
            FT_SPHERE,
            FT_CONE,
            FT_NUM
        }type;
    int index;
    float t;
};
bool operator<(const Figure l, const Figure r);
bool operator>(const Figure l, const Figure r);

class CScene
{
public:
       CCamera           camera;

       vector<CLight   > light;
       vector<CSphere  > sphere;
       vector<CCone    > cone;

       PixelMatrix       pixelMatrix; 
private:
       vector<CLight> NewLights;
       vector<Figure> Figures;
       void           CreateFigures(CRay ray); //заполнение вектора Figures.
       bool           SearchMinInFigures(vector<Figure>::iterator&); 
       void           CreateNewLightVector(fl3 Point, fl3 Normal);
public:
       void           Draw();
       void           RayTracing(); 
};

#endif