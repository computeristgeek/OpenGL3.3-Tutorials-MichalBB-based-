//********************************** OpenGL Init.cpp ******************************************
// 
//			Here are functions, that are needed to make OpenGL work properly.
//
//********************************** OpenGL Init.cpp ******************************************


#include "Main.h"


/////////////////////////////////// SETUP PIXEL FORMAT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//			  This function checks for and sets appropriate pixel format for OpenGL
//
/////////////////////////////////// SETUP PIXEL FORMAT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool SetupPixelFormat(HDC &hdc)
{
	PIXELFORMATDESCRIPTOR pfd;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = 16;						// Here we use our #define for the color bits
    pfd.cDepthBits = 16;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits

	int result=ChoosePixelFormat(g_DC,&pfd);
	if(result==FALSE)
	{
		MessageBox(NULL,"Unable to choose pixel format","Error",MB_ICONERROR | MB_OK);
		return false;
	}
	if(SetPixelFormat(g_DC,result,&pfd)==FALSE)
	{
		MessageBox(NULL,"Unable to set pixel format","Error",MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}
 
//////////////////////////////// SIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					  Resizes OpenGL screen and sets up translations.
//
//////////////////////////////// SIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SizeOpenGLScreen(int width, int height)			// Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero error
	{
		height=1;										// Make the Height Equal One
	}

	glViewport(0,0,width,height);							// Make our viewport the whole window

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluPerspective(45.0f,(float)width/(float)height,0.1f ,1500.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

////////////////////////////////////// INIT OPENGL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							This function initializes OpenGL
//
////////////////////////////////////// INIT OPENGL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitOpenGL(int w, int h)
{
	g_DC=GetDC(g_Wnd); // Get device context
	if(!SetupPixelFormat(g_DC)) // If couldn't setup pixel format, quit
		PostQuitMessage(0);

	g_RC=wglCreateContext(g_DC); // Create OpenGL rendering context
	wglMakeCurrent(g_DC,g_RC); // Set OpenGL rendering context

	SizeOpenGLScreen(w,h); // Resize OpenGL screen

	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_TEXTURE_2D); // Enable texturing

	glClearColor(1,1,1,1); // Clear color to green
}
