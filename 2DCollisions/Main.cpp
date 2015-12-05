#include "main.h"				
#include <math.h>
#include "Weapons.h"

GLint ammop;

GLboolean hrot;GLfloat x,y,xinc,yinc;
GLboolean  g_bFullScreen = TRUE;								
GLuint  g_hWnd;											
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()
GLfloat rotation;
const GLfloat piover180 = 0.0174532925f;
GLuint g_Texture[MAX_TEXTURES];	

GLvoid Init(GLuint hWnd)
{
	g_hWnd = hWnd;										
	GetClientRect(g_hWnd, &g_rRect);					
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	
CreateTexture(g_Texture,"Data/Background.bmp",0);
glEnable(GL_TEXTURE_2D);
InitAmmo();
PlayMidi("Data/Action.mid");
Ryby();
initd();
}

GLuint MainLoop()
{
	MSG msg;

	while(1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			// Render the scene every frame
			RenderScene();

        } 
	}

	return(msg.wParam);									// Return from the program
}

GLint tex=3;
GLvoid RenderScene() 
{
xinc=(float)sin(rotation*piover180) * 0.05f;
yinc=(float)cos(rotation*piover180) * 0.05f;
KeyInput();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

	if(mylife>0){
		
	gluLookAt(x, y, 6,     x, y, 0,     0, 1, 0);
DrawShip();
Craft();
DrawAmmo();
TestCollision();
Vyvolaj();
EnemyShot();
	Ortho();
	}
	else{
		glBindTexture(GL_TEXTURE_2D,g_Texture[tex]);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
glTexCoord2f(0,1); glVertex3f(-2.0f,2,-5.0f);
glTexCoord2f(1,1); glVertex3f(2.0f,2,-5.0f);
glTexCoord2f(1,0); glVertex3f(2.0f,-2,-5.0f);
glTexCoord2f(0,0); glVertex3f(-2.0f,-2,-5.0f);
glEnd();
StopMidi();
	}
	if(mylife>0 && count>=80){ mylife=-1; tex=1;}
	SwapBuffers(g_hDC);	
timer+=inc;
}

LRESULT CALLBACK WinProc(GLuint hWnd,GLuint uMsg, GLuint wParam, long lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (uMsg)
	{ 
    case WM_SIZE:										// If the window is resized
		if(!g_bFullScreen)								// Do this only if we are NOT in full screen
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);					// Get the window rectangle
		}
        break; 
 
	case WM_PAINT:										// If we need to repaGLint the scene
		BeginPaint(hWnd, &ps);							// Init the paGLint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)	DeInit();
	
		break;
 
    case WM_DESTROY:									// If the window is destroyed
        DeInit();										// Release memory and restore settings
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}
