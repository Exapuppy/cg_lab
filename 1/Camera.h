#include "fl3.h"
#pragma once

class CCamera
{
	private:
		fl3 m_vPosition;
		fl3 m_vView;
		fl3 m_vUpVector;
		fl3 m_vStrafe;
	public:	
		double kSpeed;
		bool MouseFlag;
        double Perspective;

		CCamera      ( );
		fl3 Position ( ) { return m_vPosition;	}
		fl3 View     ( ) { return m_vView;		}
		fl3 UpVector ( ) { return m_vUpVector;	}
		fl3 Strafe   ( ) { return m_vStrafe;	}
		void PositionCamera (	double PositionX,	double PositionY,	double PositionZ,
								double ViewX,		double ViewY,		double ViewZ,
								double upVectorX,	double upVectorY,	double upVectorZ );

		void StrafeCamera (	double speed );
		void MoveCamera ( double speed );
		void RotateView ( double angle, double x, double y, double z );
		void Keyboard ( );
		void Update ( int w, int h );
		void Look ( );
		void Mouse ( );
};