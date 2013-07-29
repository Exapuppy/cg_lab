#ifndef __PCH_H
#define __PCH_H

# include <windows.h>
# include <stdio.h>
# define  _USE_MATH_DEFINES
# include <math.h>
# include <time.h>
# include <GL/GL.h>
# include <GL/GLU.h>
# include "glut.h"
# include <vector>
//#pragma once

# define SCREEN_WIDTH	800
# define SCREEN_HEIGHT	600
# define eps			0.05

using namespace std;

typedef unsigned int uint;

//ray tracing or normal mode
#define KEY_PLAYMODE         'p'
//correct perspective angle
#define KEY_PERSPECTIVEPLUS  '.'
#define KEY_PERSPECTIVEMINUS ','
//draw in normal mode XYZ-axis
#define KEY_DRAWXYZ          'i'
//set light mode edition
#define KEY_SET_LIGHT_MODE   'l'

//1-9
#define KEY_1_PARAM          '1'
#define KEY_2_PARAM          '2'
#define KEY_3_PARAM          '3'

#endif