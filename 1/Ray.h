#include "fl3.h"
#include "matrix.h"

#ifndef __RAY_H
#define __RAY_H

class CRay
{
public:
    fl3 _1;//координата исходной точки
    fl3 _2;//направление луча
    CRay(fl3 _1, fl3 _2): _1(_1), _2(_2){};
    CRay(){};
    fl3 getPoint(float t);
};

CRay operator*(const matrix a, const CRay b);

#endif