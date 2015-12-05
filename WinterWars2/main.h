#ifndef _MAIN_H
#define _MAIN_H


#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>										// Header File For The OpenGL32 Library
#include <GL/glu.h>										// Header File For The GLu32 Library
#include <GL/glaux.h>	





#define SCREEN_WIDTH 800								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 600								// We want our screen height 600 pixels
#define SCREEN_DEPTH 16									// We want 16 bits per pixel

#define MAX_TEXTURES 8
extern GLuint g_Texture[MAX_TEXTURES];
GLvoid Sound(char*name);
class CVector3
{
public:
	
	// A default constructor
	CVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(GLfloat X, GLfloat Y, GLfloat Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(GLfloat num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(GLfloat num)
	{
		// Return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	GLfloat x, y, z;						
};

CVector3 Normalize(CVector3);

extern GLboolean  g_bFullScreen;									// Set full screen as default
extern GLuint  g_hWnd;										// This is the handle for the window
extern RECT  g_rRect;										// This holds the window dimensions
extern GLvoid*   g_hDC;											// General GLvoid* - (handle to device context)
extern HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
extern GLvoid* g_hInstance;								// This holds our window hInstance

// This is our MAIN() for windows
GLint WINAPI WinMain(GLvoid* hInstance, GLvoid* hprev, PSTR cmdline, GLint ishow);

// The window proc which handles all of window's messages.
LRESULT CALLBACK WinProc(GLuint hwnd, GLuint message, GLuint wParam, long lParam);

// This controls our main program loop
GLuint MainLoop();

// This changes the screen to full screen mode
GLvoid ChangeToFullScreen();

// This is our own function that makes creating a window modular and easy
GLuint CreateMyWindow(LPSTR strWindowName, GLint width, GLint height, DWORD dwStyle, GLboolean bFullScreen, GLvoid* hInstance);

// This allows us to configure our window for OpenGL and backbuffered
GLboolean bSetupPixelFormat(GLvoid* hdc);

// This inits our screen translations and projections
GLvoid SizeOpenGLScreen(GLint width, GLint height);

// This sets up OpenGL
GLvoid InitializeOpenGL(GLint width, GLint height);

// This initializes the whole program
GLvoid Init(GLuint hWnd);

// This draws everything to the screen
GLvoid RenderScene();

// This frees all our memory in our program
GLvoid DeInit();

GLvoid CreateTexture(GLuint textureArray[], LPSTR strFileName, GLint textureID);
GLvoid CreateWorld();
BOOL PlayMidi(GLchar *sFileName);
BOOL PauseMidi();
BOOL ResumeMidi();
BOOL StopMidi();
BOOL ReplayMidi();
GLvoid Textures();
GLvoid Ortho();
GLGLvoid BuildFont(GLvoid);
GLGLvoid KillFont(GLvoid);

GLGLvoid glPrintFast(GLGLint x, GLGLint y, GLchar *string, GLint set);
extern GLuint a[2];
GLuGLint LoadGLTexture( const GLchar *filename );
extern GLint ammol;
GLvoid BuildAmmo();
#endif 
