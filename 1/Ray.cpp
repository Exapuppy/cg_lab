#include "Ray.h"

CRay operator*(const matrix a, const CRay b)
{
    CRay res; 
    res._1 = a*b._1;
    if(a.type != matrix::TR) 
        res._2 = a*b._2;
    else
        res._2 = b._2;
    return res;
}

fl3 CRay::getPoint(float t)
{
    return _1 + _2*t;
}