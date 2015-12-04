#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>										// Header File For The OpenGL32 Library
#include <GL/glu.h>										// Header File For The GLu32 Library
#include <GL/glaux.h>
#include <math.h>	
#define SCREEN_WIDTH 800								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 600								// We want our screen height 600 pixels
#define SCREEN_DEPTH 16									// We want 16 bits per pixel
#define MAX_TEXTURES 2
extern UINT g_Texture[MAX_TEXTURES];
// *EXTERNING VARIABLES*
BOOL PlayMidi(char *sFileName);
BOOL StopMidi();
extern GLboolean  g_bFullScreen;								// Set full screen as default
extern HWND  g_hWnd;									// This is the handle for the window
extern RECT  g_rRect;									// This holds the window dimensions
extern HDC   g_hDC;										// General HDC - (handle to device context)
extern HGLRC g_hRC;										// General OpenGL_DC - Our Rendering Context for OpenGL
extern HINSTANCE g_hInstance;							// This holds our window hInstance

GLint WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, GLint ishow);

// The window proc which handles all of window's messages.
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// This controls our main program loop
WPARAM MainLoop();

// This changes the screen to full screen mode
GLvoid ChangeToFullScreen();

// This is our own function that makes creating a window modular and easy
HWND CreateMyWindow(LPSTR strWindowName, GLint width, GLint height, DWORD dwStyle, GLboolean bFullScreen, HINSTANCE hInstance);

// This allows us to configure our window for OpenGL and backbuffered
GLboolean bSetupPixelFormat(HDC hdc);

// This inits our screen translations and projections
GLvoid SizeOpenGLScreen(GLint width, GLint height);

// This sets up OpenGL
GLvoid InitializeOpenGL(GLint width, GLint height);

// This initializes the whole program
GLvoid Init(HWND hWnd);

// This draws everything to the screen
GLvoid RenderScene();

// This frees all our memory in our program
GLvoid DeInit();
GLvoid CreateTexture(UINT textureArray[], LPSTR strFileName, GLint textureID);
extern GLfloat timer,inc;
#endif 

