//					  MEGABYTE SOFTWORKS PROGRAMMING BASECODE
//
//     Author: Michalbb ( michalbb@centrum.sk )
//     License: You may use this basecode in non-commercial games
//
//---------------------------------------------------------------------------------------------

//*************************************** Main.h **********************************************
// 
//					Main header file, included to all source files.
//
//*************************************** Main.h **********************************************

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <float.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

#include "Fmod.h"
#include "CVector2.h"
#include "Font.h"
#include "Items.h"
#include "resource.h"

#define class_name "Fight for life"

extern GLuint g_Wnd;		  // Our global window variable used for lots of functions
extern HINSTANCE g_Inst; // Global application instance variable
extern HDC g_DC;		// Global device context variable
extern HGLRC g_RC;      // Global rendering context variable

GLvoid UpdateFPS5();

// Initializes whole application
	GLvoid Init();
// Main message loop
	GLuint MainLoop();
// Draws whole program
	GLvoid RenderScene();
// Freeing memory, variables and everything in the end of application
	GLvoid QuitApplication();
// Window message handling
	LRESULT CALLBACK WndProc(GLuint hWnd,GLuint uMsg, GLuint wParam, long lParam);
// Initializes OpenGL
	GLvoid InitOpenGL(GLint w, GLint h);
	extern GLboolean quitapp;
// Key pressed only one time
	GLboolean onekey(GLint keyid);
// Key pressed
	GLboolean keys(GLint keyid);
// Optimalized float
	GLfloat f(GLfloat inc);
//----------------------------------------------------------------------------------------------

GLvoid PlayIntro(),PlayOutro();