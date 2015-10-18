# include "fl3.h"

fl3  operator +  (const fl3 l, const fl3 r) 
{
	return fl3(l.m[0] + r.m[0],l.m[1] + r.m[1],l.m[2] + r.m[2]); 
}
fl3  operator -  (const fl3 a)
{
	return fl3(-a.m[0],-a.m[1],-a.m[2]);
}
fl3  operator -  (const fl3 l, const fl3 r)		{ return l + (-r); }
fl3  operator *  (const fl3 a, const float b) { return fl3(b*a.m[0],b*a.m[1],b*a.m[2]); }
fl3  operator *  (const GLfloat a, const fl3 b) { return b*(float)a; }
//покомпонентное произведение
fl3 operator ^ (const fl3 a, const fl3 b) { return fl3(a.m[0] * b.m[0], a.m[1] * b.m[1], a.m[2] * b.m[2]); }
float operator *  (const fl3 a, const fl3 b) { return (a.m[0]*b.m[0] + a.m[1]*b.m[1] + a.m[2]*b.m[2]); }
fl3   operator /  (const fl3 a, const GLfloat b) { return a*(1.0f/b); }
bool  operator == (const fl3 a, const fl3 b) { return a.m[0]==b.m[0]&&a.m[1]==b.m[1]&&a.m[2]==b.m[2]; }
bool  operator &  (const fl3 a, const fl3 b) { 
    fl3 aa = normalize(a), bb = normalize(b); 
    return aa.m[0]==bb.m[0]&&aa.m[1]==bb.m[1]&&aa.m[2]==bb.m[2]; 
}
bool  operator >  (const fl3 a, const fl3 b) {return a.m[0]>b.m[0]&&a.m[1]>b.m[1]&&a.m[2]>b.m[2];}

void fl3::operator += (const fl3 r)		{ *this = *this + r; }
void fl3::operator -= (const fl3 r)		{ *this = *this - r; }
void fl3::operator *= (const GLfloat r) { *this = *this * r; }
void fl3::operator /= (const GLfloat r) { *this = *this / r; }

fl3 ort ( fl3 v1, fl3 v2 )
{
	return fl3(	( ( v1.m[1] * v2.m[2] ) - ( v1.m[2] * v2.m[1] ) ),
				( ( v1.m[2] * v2.m[0] ) - ( v1.m[0] * v2.m[2] ) ),
				( ( v1.m[0] * v2.m[1] ) - ( v1.m[1] * v2.m[0] ) ) );
}

fl3 normalize ( fl3 v )
{
	float x = v.m[0], y = v.m[1], z = v.m[2];
	return v/sqrt(x*x+y*y+z*z);
}

fl3 norm_ort( fl3 v1, fl3 v2)
{
	return normalize(ort(v1,v2));
}

float norm( fl3 v )
{
	float x = v.m[0], y = v.m[1], z = v.m[2];
	return sqrt(x*x+y*y+z*z);
}

fl3 fl3_rotate ( float Angle, fl3 NormalVector, fl3 Vector )
{

#define RAD M_PI/360
	fl3 New;
	float	cosTheta = ( float ) cos ( Angle*RAD ),
			sinTheta = ( float ) sin ( Angle*RAD ),
			_1_cosTh = 1 - cosTheta,
			x = NormalVector.m[0],
			y = NormalVector.m[1],
			z = NormalVector.m[2],
			_1_cosTh_xy = _1_cosTh * x * y,
			_1_cosTh_yz = _1_cosTh * z * y,
			_1_cosTh_xz = _1_cosTh * z * x,
			z_sinTh = z*sinTheta,
			y_sinTh = y*sinTheta,
			x_sinTh = x*sinTheta;

	New.m[0]	 =  ( cosTheta + _1_cosTh * x * x ) * Vector.m[0] +
					( _1_cosTh_xy - z_sinTh )		* Vector.m[1] +
					( _1_cosTh_xz + y_sinTh )		* Vector.m[2];

	New.m[1]	=	( _1_cosTh_xy + z_sinTh )		* Vector.m[0] +
					( cosTheta + _1_cosTh * y * y )	* Vector.m[1] +
					( _1_cosTh_yz - x_sinTh )		* Vector.m[2];

	New.m[2]	=	( _1_cosTh_xz - y_sinTh )		* Vector.m[0] +
					( _1_cosTh_yz + x_sinTh )		* Vector.m[1] +
					( cosTheta + _1_cosTh * z * z ) * Vector.m[2];
	
	return New;
}

fl3 to_spherical_r_phi_theta(fl3 point)
{
    float x     = point.m[0],
           y     = point.m[1],
           z     = point.m[2],
           r     = sqrt(x*x + y*y + z*z),
           theta = acos(z/r),
           phi   = atan(y/x);
    return fl3(r,phi,theta);
}

fl3 to_cylindrical_r_phi_z(fl3 point)
{
    float x     = point.m[0],
           y     = point.m[1],
           z     = point.m[2],
           r     = sqrt(x*x + y*y),
           phi   = atan(y/x);
    return fl3(r,phi,z);
}