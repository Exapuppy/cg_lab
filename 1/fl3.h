#ifndef __FL3_H
#define __FL3_H
#include "pch.h"

//декартовы координаты
class fl3
{
public:
	GLfloat m[3];
	void operator= (fl3 r)
	{
		this->m[0] = r.m[0];
		this->m[1] = r.m[1];
		this->m[2] = r.m[2];
	}
	fl3(float _1, float _2, float _3)
	{
		m[0] = _1; m[1] = _2; m[2] = _3;
	}
	fl3()
	{
		m[0] = 0; m[1] = 0; m[2] = 0;
	}
	void operator += (const fl3 r);
	void operator -= (const fl3 r);
	void operator *= (const GLfloat r);
	void operator /= (const GLfloat r);
};

class fl2
{
public:
	GLfloat m[2];
	void operator = (fl2 r) { m[0] = r.m[0]; m[1] = r.m[1]; }
    fl2(float _1, float _2) { m[0] = _1    ; m[1] = _2    ; }
	fl2()                   { m[0] = 0     ; m[1] = 0     ; }
};

fl3   operator +  (const fl3     l, const fl3     r);
fl3   operator -  (const fl3     a                 );
fl3   operator -  (const fl3     l, const fl3     r);
fl3   operator *  (const fl3     a, const GLfloat b);
fl3   operator *  (const GLfloat a, const fl3     b);
//покомпонентное произведение
fl3   operator ^  (const fl3 a, const fl3     b);
float operator *  (const fl3 a, const fl3     b);
fl3   operator /  (const fl3 a, const GLfloat b);
bool  operator == (const fl3 a, const fl3     b);
//параллельны ли
bool  operator &  (const fl3 a, const fl3     b);
//больше ли каждая компонента левого каждого компонента правого
bool  operator >  (const fl3 a, const fl3     b); 

//возвращает ортогональный v1 и v2 вектор
fl3   ort ( fl3 v1, fl3 v2 );
//нормализует вектор
fl3   normalize ( fl3 v );
//возвращает нормализованный ортогональный векторам v1 и v2 вектор
fl3   norm_ort( fl3 v1, fl3 v2);
float norm( fl3 v );
/* возвращает Vector, повернутый вокруг вектора NormalVector
 * на угол Angle
 */
fl3   fl3_rotate ( float Angle, fl3 NormalVector, fl3 Vector );

//преобразования в сферическую и цилиндрическую системы координат.
fl3   to_spherical_r_phi_theta(fl3 point);
fl3   to_cylindrical_r_phi_z(fl3 point);

#endif