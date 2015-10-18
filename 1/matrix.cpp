#include "matrix.h"

matrix::matrix()
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++)
      m[i][j] = 0;
  m[0][0] = 1;
  m[1][1] = 1;
  m[2][2] = 1;
}

void matrix::operator*=(fl3 a)//scale
{
  m[0][0] = 1.0f / a.m[0];
  m[1][1] = 1.0f / a.m[1];
  m[2][2] = 1.0f / a.m[2];

  type = SC;
}

void matrix::operator*=(float a)//scale
{
  m[0][0] = m[1][1] = m[2][2] = 1.0f / a;

  type = SC;
}

void matrix::operator+=(fl3 a)//translate
{
  m[0][3] -= a.m[0];
  m[1][3] -= a.m[1];
  m[2][3] -= a.m[2];

  type = TR;
}

void matrix::operator&=(fl3 normal)//rotate
{
  fl3 up = fl3(0, 1, 0);
  if (normal & up)
    return;
  fl3 U = norm_ort(normal, up);
  float c = (up*normal) / (norm(normal)),
    s = sqrt(1 - c*c);
  float ux = U.m[0], uy = U.m[1], uz = U.m[2];

  m[0][0] = (1 - c)*ux*ux + c; m[0][1] = (1 - c)*uy*ux - s*uz; m[0][2] = (1 - c)*uz*ux - s*uy;
  m[0][0] = (1 - c)*ux*uy + s*uz; m[0][1] = (1 - c)*uy*uy + c; m[0][2] = (1 - c)*uz*uy - s*ux;
  m[0][0] = (1 - c)*ux*uz - s*uy; m[0][1] = (1 - c)*uy*uz + s*ux; m[0][2] = (1 - c)*uz*uz + c;

  type = RT;
}

fl3 operator*(const matrix a, const fl3 b)
{
  return fl3(a.m[0][0] * b.m[0] + a.m[0][1] * b.m[1] + a.m[0][2] * b.m[2] + a.m[0][3],
    a.m[1][0] * b.m[0] + a.m[1][1] * b.m[1] + a.m[1][2] * b.m[2] + a.m[1][3],
    a.m[2][0] * b.m[0] + a.m[2][1] * b.m[1] + a.m[2][2] * b.m[2] + a.m[2][3]);
}