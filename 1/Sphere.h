#include "Object.h"

class CSphere : public CObject
{
private:
  void create();
public:
  CSphere();
  CSphere(float _r);
  CSphere(float _r, fl3 _c);
  float r;

  void Draw();
  ///////////////////////////////
private:
  void calculateTexCoord();
  void calculatePointColor();
  void calculateIntensity(vector<CLight> Lights);
  fl3 calculateIntensity_Phong(fl3 a, fl3 d, fl3 s, fl3 LightPos);
  fl3 calculateIntensity_Cook_Torrens(fl3 a, fl3 d, fl3 s, fl3 LightPos);

  fl3  TransferToCanonical(fl3 point);
  CRay TransferToCanonical(CRay ray);
public:
  bool trace(CRay ray);
  fl3 getNormal(fl3 Eye);
  fl3 getColor(vector<CLight> Lights, int SceneLightCount);
  ////////////////////////////////

  bool trace(CRay ray, float& t);
};
