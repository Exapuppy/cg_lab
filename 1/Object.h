# include "fl3.h"
# include "matrix.h"
# include "Ray.h"
# include "Light.h"
# include "Material.h"
# pragma once

# define VERTEX_NUM 16

class CObject
{
public:
  fl3	c;//центр объекта
  bool b_create;//был ли рассчитан каркас объекта
  vector<vector<fl3>> obj;//каркас объекта
  virtual void Draw() = 0;//отрисовка каркаса объекта
  virtual void create() = 0;//создание каркаса объекта
protected:
  fl3         SavedNormal;  //
  fl3         SavedPoint;   //canonical
  fl2         SavedTexCoord;//
  fl3         SavedColor;   //
  CRay        SavedEyeRay;  //canonical
  vector<fl3> SavedIntensity;//
  CObjectLightParam *LightCoef;
  matrix transpose,
    rotate,
    scale;

  /* Рассчитывает цвет в точке SavedPoint,
   * основываясь на SavedTexCoord
   */
  virtual void calculatePointColor() = 0;
  /* Рассчитывает интенсивность цвета в точке SavedPoint
   * с помощью модели освещения Кука-Торренса
   */
  virtual void  calculateIntensity(vector<CLight> Lights) = 0;
  virtual fl3
    calculateIntensity_Phong(fl3 a, fl3 d, fl3 s, fl3 LightPos) = 0;
  virtual fl3
    calculateIntensity_Cook_Torrens(fl3 a, fl3 d, fl3 s, fl3 LightPos) = 0;

  virtual fl3  TransferToCanonical(fl3 point) = 0;
  virtual CRay TransferToCanonical(CRay ray) = 0;
public:
  virtual fl3  getNormal(fl3 Eye) = 0;
  virtual fl3  getColor(vector<CLight> Lights, int SceneLightCount) = 0;
  virtual bool trace(CRay ray, float& t) = 0;
  virtual bool trace(CRay ray) = 0;
};