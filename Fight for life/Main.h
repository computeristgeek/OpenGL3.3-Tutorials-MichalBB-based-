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
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
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

extern HWND g_Wnd;		  // Our global window variable used for lots of functions
extern HINSTANCE g_Inst; // Global application instance variable
extern HDC g_DC;		// Global device context variable
extern HGLRC g_RC;      // Global rendering context variable

void UpdateFPS5();

// Initializes whole application
	void Init();
// Main message loop
	WPARAM MainLoop();
// Draws whole program
	void RenderScene();
// Freeing memory, variables and everything in the end of application
	void QuitApplication();
// Window message handling
	LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
// Initializes OpenGL
	void InitOpenGL(int w, int h);
	extern bool quitapp;
// Key pressed only one time
	bool onekey(int keyid);
// Key pressed
	bool keys(int keyid);
// Optimalized float
	float f(float inc);
//----------------------------------------------------------------------------------------------

void PlayIntro(),PlayOutro();