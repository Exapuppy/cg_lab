#pragma comment (lib, "glaux.lib")
#include "Scene.h"


int  width  = SCREEN_WIDTH, 
	 height = SCREEN_HEIGHT;
bool b_XYZ  = false;

CScene scene;

enum PLAY_MODE{PM_PAUSE,PM_PLAY} playMode = PM_PLAY;

void timf(int value)
{
    scene.camera.Update( width, height );
    scene.camera.Keyboard();
	if(playMode == PM_PAUSE) 
    {
        scene.RayTracing();
        glutPostRedisplay	();
        glutTimerFunc	(20, timf, 0);
    }
    else
    {
        glutPostRedisplay	();
        glutTimerFunc	(20, timf, 0);
    }
}
void DrawXYZ (int x, int y, int z )
{
	glLineWidth (2);
	glNormal3f(0,1,0);
	glBegin ( GL_LINES );
		glColor3ub ( 255, 0, 0 );
		glVertex3f ( x, y, z );
		glVertex3f ( x+20, y, z );
		glColor3ub ( 0, 255, 0 );
		glVertex3f ( x, y, z );
		glVertex3f ( x, y+20, z );
		glColor3ub ( 0, 0, 255 );
		glVertex3f ( x, y, z );
		glVertex3f ( x, y, z+20 );
	glEnd ( );
    glLineWidth (1);
}
void display(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

    if(playMode == PM_PAUSE) 
    {
        glDrawPixels(
                        scene.pixelMatrix.width, 
                        scene.pixelMatrix.height, 
                        GL_RGB,
                        GL_FLOAT, 
                        scene.pixelMatrix.pixels
                    );
    }
    else
    {
        scene.camera.Look();
        scene.Draw();
    
        if(b_XYZ) DrawXYZ(0,0,0);
    }

	glPopMatrix();
	glutSwapBuffers();
}
void resize(int width,int height)
{
	::width = width; ::height = height;
	glViewport( 0, 0, width, height );

    if(playMode == PM_PAUSE) 
    {
        scene.pixelMatrix.width = width;
        scene.pixelMatrix.height = height;

        {
            int n = height*width;
            delete[] scene.pixelMatrix.pixels;
            scene.pixelMatrix.pixels = new float[3*n];
        }
        
        scene.RayTracing();
        glutPostRedisplay();
    }
    else
    {
        scene.camera.Update( width, height );
	    scene.camera.Look( );
    }
    
	glMatrixMode( GL_MODELVIEW );
}
void keyboard( unsigned char key, int x, int y )
{
    static enum{Knone,Klight,Kfigure,Ktex,Kmaterial}key_mode = Klight;

	switch( key ) {	
	case '\033': exit(0);
    case KEY_PLAYMODE: 
        playMode = playMode ? PM_PAUSE : PM_PLAY; 
        if(playMode == PM_PLAY) glutTimerFunc	(20, timf, 0);
        else 
        {
            scene.pixelMatrix.width = width;
            scene.pixelMatrix.height = height;
            scene.RayTracing();
            glutPostRedisplay();
        }
        break;
    case KEY_PERSPECTIVEPLUS: 
        if(scene.camera.Perspective < 160.0) 
        {
            scene.camera.Perspective += 0.5; 
            if(playMode == PM_PAUSE) 
            {
                scene.RayTracing();
                glutPostRedisplay();
            }
        }
        break;
    case KEY_PERSPECTIVEMINUS: 
        if(scene.camera.Perspective > 10.0)
        {
            scene.camera.Perspective -= 0.5; 
            if(playMode == PM_PAUSE) 
            {
                scene.RayTracing();
                glutPostRedisplay();
            }
        }
        break;
    case KEY_DRAWXYZ: 
        b_XYZ = !b_XYZ;
        break;
    case KEY_SET_LIGHT_MODE:
        key_mode = Klight;
        break;
	default: break;
	}
    
    switch(key_mode)
    {
    case Klight:
        {
            switch( key ) 
            {
            case KEY_1_PARAM: 
                CLight::light_model = CLight::LM_NO_LIGHT;
                break;
            case KEY_2_PARAM: 
                CLight::light_model = CLight::LM_PHONG;
                break;
            case KEY_3_PARAM:
                CLight::light_model = CLight::LM_COOK_TORRENS;
                break;
            }
        }
        break;
    default: 
        break;
    }
}
void mouse( int button, int state, int x, int y )
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN )//&& playMode == PM_PLAY)
	{
		scene.camera.MouseFlag = !scene.camera.MouseFlag;
	}
}
void init()
{
	glutInitWindowPosition	( 450, 200 );
	glutInitWindowSize		( 500, 400 );
	glutInitDisplayMode		( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
	glutCreateWindow		( "l3:0" );
	glutDisplayFunc			( display );
	glutReshapeFunc			( resize );
	glutKeyboardFunc		( keyboard );
	glutMouseFunc			( mouse );
	glutTimerFunc			( 20, timf, 0 );

	glEnable(GL_DEPTH_TEST);		//Включение глубины
	glEnable(GL_DOUBLEBUFFER);		//Двойной буфер кадра

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.01f, 40000.0f );

	scene.camera.Look();			//Установка точки обзора

    glClearColor(BackColor.m[0],
                 BackColor.m[1],
                 BackColor.m[2],
                 1             );	//Установка фонового цвета

    glLineWidth (1);
    glColor3d(0.1,0.35,0.1);
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
}
void main()
{
    scene.light .push_back(CLight(fl3(170,0,0)));

    scene.cone  .push_back(CCone(5,0,20,fl3(40,0,5))); 
    scene.sphere.push_back(CSphere(5, fl3(60,0,0)));
    scene.sphere.push_back(CSphere(30, fl3(0,0,0)));
    scene.sphere.push_back(CSphere(5, fl3(0,30,0)));
    
	init();
	glutMainLoop();
}