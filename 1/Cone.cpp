#include "Cone.h"

CCone::CCone()
{
  b_create = false;
  obj.resize(VERTEX_NUM);
  for (int i = 0; i < VERTEX_NUM; i++) obj[i].resize(VERTEX_NUM);
  c = fl3(0, 0, 0);
  LightCoef = new CObjectLightParam();
}
CCone::CCone(float _r1, float _r2, float _h)
{
  b_create = false;
  r1 = _r1;
  r2 = _r2;
  h = _h;
  obj.resize(VERTEX_NUM);
  for (int i = 0; i < VERTEX_NUM; i++) obj[i].resize(VERTEX_NUM);
  c = fl3(0, 0, 0);
  LightCoef = new CObjectLightParam();
}
CCone::CCone(float _r1, float _r2, float _h, fl3 _c)
{
  b_create = false;
  r1 = _r1;
  r2 = _r2;
  h = _h;
  obj.resize(VERTEX_NUM);
  for (int i = 0; i < VERTEX_NUM; i++) obj[i].resize(VERTEX_NUM);
  c = _c;
  LightCoef = new CObjectLightParam();
}
void CCone::create()
{
  //for OGL drawing
  static const float phi_end = 2.0f*(float)M_PI, d_phi = (2.0f*(float)M_PI / (float)VERTEX_NUM);
  float phi = 0;
  for (int i = 0; phi < phi_end && i < VERTEX_NUM; phi += d_phi, i++)
  {
    float r = r1, r_end = r2, d_r = ((r2 - r1) / (float)VERTEX_NUM);
    float hh = -h*0.5f, hh_end = h*0.5f, d_hh = (h / (float)VERTEX_NUM);
    for (int j = 0; hh < hh_end && j < VERTEX_NUM; r += d_r, j++, hh += d_hh)
    {
      obj[i][j] = c + (fl3(r*cos(phi), hh, r*sin(phi)));
    }
  }

  //create matrix for ray tracing
  transpose += (c - fl3(0.0f, h*0.5f, 0.0f));
  float  R = max(r1, r2);
  scale *= fl3(R, h, R);
}
void CCone::Draw()
{
  if (!b_create) { create(); b_create = true; }

  glBegin(GL_LINES);
  for (uint i = 0; i < obj.size(); i++)
  {
    for (uint j = 0, j1 = 1; j1 < obj[i].size(); j++, j1++)
    {

      glVertex3fv(obj[i][j].m);
      glVertex3fv(obj[i][j1].m);
      glVertex3fv(obj[j][i].m);
      glVertex3fv(obj[j1][i].m);
    }
  }
  for (uint j = 0, n = obj.size() - 1; j < obj[0].size(); j++)
  {
    glVertex3fv(obj[0][j].m);
    glVertex3fv(obj[n][j].m);
  }
  glEnd();
}



fl3  CCone::TransferToCanonical(fl3 point)
{
  return scale*(rotate*(transpose*point));
}
CRay CCone::TransferToCanonical(CRay ray)
{
  return scale*(rotate*(transpose*ray));
}
bool  CCone::trace(CRay ray, float& t)
{
  static float  R = r1,//max(r1,r2),
    r = r2;//min(r1,r2);
  CRay Ray = TransferToCanonical(ray);
  SavedEyeRay = ray;
  float Dirx = Ray._2.m[0], Diry = Ray._2.m[2], Dirz = Ray._2.m[1],
    Eyex = Ray._1.m[0], Eyey = Ray._1.m[2], Eyez = Ray._1.m[1],

    s = r / R, k = s - 1, d = k*Dirz, f = 1 + k*Eyez,

    A = Dirx*Dirx + Diry*Diry - d*d,
    B = (Eyex*Dirx + Eyey*Diry - f*d) * 2,
    C = Eyex*Eyex + Eyey*Eyey - f*f;

  float D = B*B - 4.0f*A*C;

  if (D >= 0) //бокова€ поверхность
  {
    t = (-B - sqrtf(D)) / (2.0f*A);
    fl3 P = Ray.getPoint(t);

    if (t < 0) return false;

    fl3 Vscale = fl3(R, h, R),

      P1 = (Ray._1) ^ Vscale,
      P2 = P ^ Vscale;

    float norm1 = norm(P1 - P2);
    float norm2 = norm(ray._2);
    t = norm1 / norm2;

    SavedPoint = TransferToCanonical(ray.getPoint(t));

    SavedNormal = normalize(fl3(SavedPoint.m[0], (R - r) / h, SavedPoint.m[2]));

    if (P.m[1] <= 1 && P.m[1] >= 0) return true;
  }

  float t1 = -Eyez / Dirz,    //нижнее основание 
    t2 = t1 + 1.0f / Dirz; //верхнее
  t = min(t1, t2);
  fl3   P = Ray.getPoint(t);

  if (t < 0) return false;

  fl3 Vscale = fl3(R, h, R),
    P1 = (Ray._1) ^ Vscale,
    P2 = P ^ Vscale;
  float norm1 = norm(P1 - P2);
  float norm2 = norm(ray._2);
  t = norm1 / norm2;

  SavedPoint = TransferToCanonical(ray.getPoint(t));

  float px = P.m[0],
    py = P.m[2];
  if (t1 < t2)
  {
    SavedNormal = fl3(0, -1, 0);
    return (px*px + py*py <= 1);
  }
  else
  {
    SavedNormal = fl3(0, 1, 0);
    return (px*px + py*py <= s*s);
  }
}

void CCone::calculatePointColor()
{
  SavedColor = fl3(0.8f, 0.8f, 0.8f);
}
void CCone::calculateIntensity(vector<CLight> Lights)
{
  //рассчитываютс€ диффузные, спекул€рные и зеркальные интенсивности
  //по модели ‘онга.
  SavedIntensity.clear();
  int n = Lights.size();
  SavedIntensity.resize(n);
  for (int i = 0; i < n; i++)
  {
    CLight l = Lights[i];
    fl3 pos = TransferToCanonical(l.pos);
    fl3 _a = l.Ia ^ LightCoef->ka,
      _d = l.Id ^ LightCoef->kd,
      _s = l.Is ^ LightCoef->ks;
    SavedIntensity[i] = (CLight::light_model == CLight::LM_PHONG)
      ? calculateIntensity_Phong(_a, _d, _s, pos)
      : calculateIntensity_Cook_Torrens(_a, _d, _s, pos);
  }
}
fl3  CCone::calculateIntensity_Phong(fl3 a, fl3 d, fl3 s, fl3 LightPos)
{
  fl3 n = SavedNormal,
    cp = SavedPoint,
    S = normalize(LightPos - cp),
    r = fl3_rotate((float)M_PI, n, S),
    v = normalize((SavedEyeRay._1) - cp);
  return a + d*(n*S) + s*pow((r*v), LightCoef->F);
}
fl3  CCone::calculateIntensity_Cook_Torrens(fl3 a, fl3 d, fl3 s, fl3 LightPos)
{
  return SavedNormal;
}
bool CCone::trace(CRay ray)
{
  float t;
  return trace(ray, t);
}
fl3  CCone::getNormal(fl3 Eye)
{
  return SavedNormal;
}
fl3  CCone::getColor(vector<CLight> Lights, int SceneLightCount)
{
  //здесь интенсивностb света от разных источников суммируютс€. 
  if (CLight::light_model != CLight::LM_NO_LIGHT)
  {
    int n = Lights.size();
    if (n == 0) return fl3(0, 0, 0);
    calculatePointColor();
    calculateIntensity(Lights);
    fl3 col = fl3(0, 0, 0);
    for (int i = 0; i < n; i++)
    {
      fl3 clr = Lights[i].clr;
      float r = clr.m[0],
        g = clr.m[1],
        b = clr.m[2];
      fl3 intensity = SavedIntensity[i];
      float ir = intensity.m[0],
        ig = intensity.m[1],
        ib = intensity.m[2];
      col += fl3(r*ir, g*ig, b*ib);
    }
    col = SavedColor ^ col;
    //col /= (float)SceneLightCount;
    return col;
  }
  return SavedNormal;
}