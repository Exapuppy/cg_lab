#include "Light.h"

void CLight::Draw()
{
  const fl3 m[8] = {
          fl3(-1,-1,-1), fl3(-1,-1, 1),
          fl3(-1, 1,-1), fl3(-1, 1, 1),
          fl3(1,-1,-1), fl3(1,-1, 1),
          fl3(1, 1,-1), fl3(1, 1, 1)
  };
  glBegin(GL_LINES);
  for (int i = 7; i >= 0; i--)
  {
    for (int j = i - 1; j >= 0; j--)
    {
      glVertex3fv((m[i] + pos).m);
      glVertex3fv((m[j] + pos).m);
    }
  }
  glEnd();
}

CLight::LIGHT_MODEL CLight::light_model = CLight::LM_NO_LIGHT;

CLight::CLight()
{
  pos = fl3(10, 0, 0);
  clr = fl3(1.0000, 1.0000, 1.0000);
  Ia = fl3(1.0000, 1.0000, 1.0000);
  Id = fl3(1.0000, 1.0000, 1.0000);
  Is = fl3(1.0000, 1.0000, 1.0000);
}
void CObjectLightParam::set(METALS_REFRACTION_COEFFICIENT a)
{
  ka = m_ambient[bridge[a]];
  kd = m_diffuse[bridge[a]];
  ks = m_specular[bridge[a]];
  F = m_shininess[bridge[a]];
  F0 = ::F0[a];
}
void CObjectLightParam::set(MATERIALS_COEFFICIENT a)
{
  ka = m_ambient[a];
  kd = m_diffuse[a];
  ks = m_specular[a];
  F = m_shininess[a];
  F0 = fl3(1, 1, 1);
}