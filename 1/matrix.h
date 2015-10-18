#include "fl3.h"
#pragma once

class matrix
{
public:
  float m[3][4];

  enum TP { TR, SC, RT, NOT } type;

  void operator+=(fl3 a);     //translate
  void operator*=(fl3 a);     //scale
  void operator*=(float a);   //scale
  void operator&=(fl3 normal);//rotate
  matrix();
};

fl3 operator*(const matrix a, const fl3 b);