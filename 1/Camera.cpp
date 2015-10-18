# include "Camera.h"

#define CAMERA_POS  fl3(0.0,0.0,30.0)
#define CAMERA_VIEW fl3(0.0,0.0,0.0)

float g_FrameInterval = 40.0f;

CCamera :: CCamera ( )
{
	kSpeed = 0.05f;
	MouseFlag = false;
	
	/*m_vPosition = fl3 ( 0.0, 0.0, 50.0 );
	m_vView		= fl3 ( 0.0, 0.0, 0.0 );
	m_vUpVector = fl3 ( 0.0, 1.0, 0.0 );*/
    m_vPosition = CAMERA_POS;
	m_vView		= CAMERA_VIEW;
	m_vUpVector = fl3 ( 0.0, 1.0, 0.0 );

    Perspective = 45.0f;
}

POINT get_cursor_pos(HWND hWnd)
{
	POINT cp;	RECT r;
	GetClientRect(hWnd, &r);
	GetCursorPos(&cp);
	cp.x -= r.left;
	cp.y -= r.top;
	return cp;
}

void CCamera :: Mouse ()
{
	HWND          hWnd = GetActiveWindow();
	POINT		  mousePos = get_cursor_pos ( hWnd );
	RECT		  rt;	GetWindowRect ( hWnd, &rt );
	int           mX = ( rt.left + rt.right ) >> 1;
	int           mY = ( rt.top + rt.bottom ) >> 1;
	
	float         angleY = 0.0f;
	float         angleZ = 0.0f;
	static float  currentRotX = 0.0f;

	if ( ( mousePos.x == mX) && ( mousePos.y == mY ) )
		return;

	SetCursorPos ( mX, mY );

	angleY = ( float ) ( ( mX - mousePos.x ) ) / 1000.0f;
	angleZ = ( float ) ( ( mY - mousePos.y ) ) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;

	if ( currentRotX > 1.0f )
	{
		currentRotX = 1.0f;
		if ( lastRotX < 1.0f )
		{
			fl3 vAxis = norm_ort ( m_vView - m_vPosition, m_vUpVector );
			RotateView ( 1.0f - lastRotX, vAxis.m[0], vAxis.m[1], vAxis.m[2] );
		}
	} else {
		if ( currentRotX < -1.0f ) {
			currentRotX = -1.0f;
			if ( lastRotX < -1.0f )
			{
				fl3 vAxis = norm_ort ( m_vView - m_vPosition, m_vUpVector );
				RotateView ( -1.0f - lastRotX, vAxis.m[0], vAxis.m[1], vAxis.m[2] );
			}
		} else {
			fl3 vAxis = norm_ort ( m_vView - m_vPosition, m_vUpVector );
			RotateView ( angleZ, vAxis.m[0], vAxis.m[1], vAxis.m[2] );
		}
	}
	RotateView ( angleY, 0, 1, 0 );
}
void CCamera :: RotateView ( float angle, float x, float y, float z )
{
	fl3 vNewView;
	fl3 vView = m_vView - m_vPosition;
	
	float	cosTheta = ( float ) cos ( angle ),
			sinTheta = ( float ) sin ( angle ),
			_1_cosTh = 1 - cosTheta,
			_1_cosTh_xy = _1_cosTh * x * y,
			_1_cosTh_yz = _1_cosTh * z * y,
			_1_cosTh_xz = _1_cosTh * z * x,
			z_sinTh = z*sinTheta,
			y_sinTh = y*sinTheta,
			x_sinTh = x*sinTheta;

	vNewView.m[0]	 =  ( cosTheta + _1_cosTh * x * x ) * vView.m[0] +
					( _1_cosTh_xy - z_sinTh )		* vView.m[1] +
					( _1_cosTh_xz + y_sinTh )		* vView.m[2];

	vNewView.m[1]	=	( _1_cosTh_xy + z_sinTh )		* vView.m[0] +
					( cosTheta + _1_cosTh * y * y )	* vView.m[1] +
					( _1_cosTh_yz - x_sinTh )		* vView.m[2];

	vNewView.m[2]	=	( _1_cosTh_xz - y_sinTh )		* vView.m[0] +
					( _1_cosTh_yz + x_sinTh )		* vView.m[1] +
					( cosTheta + _1_cosTh * z * z ) * vView.m[2];
	
	m_vView = m_vPosition + vNewView;
}

void CCamera :: StrafeCamera ( float speed )
{
	m_vPosition.m[0] += m_vStrafe.m[0] * speed;
	m_vPosition.m[2] += m_vStrafe.m[2] * speed;
	m_vView.m[0] += m_vStrafe.m[0] * speed;
	m_vView.m[2] += m_vStrafe.m[2] * speed;
}
void CCamera :: MoveCamera ( float speed )
{
	fl3 vVector = normalize ( m_vView - m_vPosition ) * speed;
	
	m_vPosition.m[0] += vVector.m[0];
	m_vPosition.m[1] += vVector.m[1];
	m_vPosition.m[2] += vVector.m[2];
	m_vView.m[0] += vVector.m[0];
	m_vView.m[1] += vVector.m[1];
	m_vView.m[2] += vVector.m[2];
}
void CCamera :: Update ( int width, int height )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(Perspective,(GLfloat)width/(GLfloat)height, 0.01f, 40000.0f );

	m_vStrafe = norm_ort( m_vView - m_vPosition, m_vUpVector );

	if(MouseFlag) 
	{
		Mouse	 ( );
	}
    fl3 dir = m_vView - m_vPosition;
}

void CCamera :: Keyboard ( )
{
	if(MouseFlag)
	{
		float speed = kSpeed * g_FrameInterval;
		if ( ( GetKeyState ( VK_UP ) & 0x80 ) || ( GetKeyState ( 'W' ) & 0x80 ) )	 
			MoveCamera ( speed );
		if ( ( GetKeyState ( VK_DOWN ) & 0x80 ) || ( GetKeyState ( 'S' ) & 0x80 ) )	 
			MoveCamera ( - speed );
		if ( ( GetKeyState ( VK_LEFT ) & 0x80 ) || ( GetKeyState ( 'A' ) & 0x80 ) )	 
			StrafeCamera (-speed);
		if ( ( GetKeyState ( VK_RIGHT ) & 0x80 ) || ( GetKeyState ( 'D' ) & 0x80 ) ) 
			StrafeCamera (speed);
	}
}

void CCamera :: Look ( )
{
	gluLookAt ( m_vPosition.m[0],	m_vPosition.m[1],	m_vPosition.m[2],
				m_vView.m[0],		m_vView.m[1],		m_vView.m[2],
				m_vUpVector.m[0],	m_vUpVector.m[1],	m_vUpVector.m[2]);
}
GLvoid CCamera :: PositionCamera ( float PositionX, float PositionY, float PositionZ,
							float ViewX, float ViewY, float ViewZ,
							float upVectorX, float upVectorY, float upVectorZ )
{

	m_vPosition = fl3(PositionX, PositionY, PositionZ);
	m_vView     = fl3(ViewX,     ViewY,     ViewZ    );
	m_vUpVector = fl3(upVectorX, upVectorY, upVectorZ);
}