#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include "main.h"									
#include "Camera.h"
#include "Weapons.h"
#include "Model.h"
#include "MilkshapeModel.h"

bool keys[5];


								
UINT g_Texture[MAX_TEXTURES];
bool one=TRUE;
bool  g_bFullScreen = true;	
float timer=0.0f;							// Set full screen as default
float inc=0.0f;
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()
bool menu=true;
int lasercount,rocketcount,plasmacount,enemyammo;
float rotate;
int win=0;
 
void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect
Textures();
	g_Camera.PositionCamera(0, 1.75f, 6,   0, 1.75f, 0,   0, 1, 0);
	PlayMidi("Data/Music/Menu.mid");
	BuildFont();
Enemy.x=-45;
Enemy.z=-38;
Enemy.life=100;
ammol=70;
ammorocket=5;
ammoplasma=10;
glClearColor(0.5f,0.5f,0.5f,0);
}


float camerax,cameraz;



WPARAM MainLoop()
{ 
	MSG msg;
LoadModels();
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
			g_Camera.Update();						
			RenderScene();	
			
	if(one){
	one=FALSE;
LoadTextures();
}
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

        } 
	}
	
	DeInit();											// Clean up and free all allocated memory

	return(msg.wParam);									// Return from the program
}
float z;

int myfrag,compfrag,fragsall=1;

char compfrags[100];
char myfrags[100];
char frags[100];
bool addone=true;
bool subone=true;

void ReInit(){
g_Camera.PositionCamera(0, 1.75f, 6,   0, 1.75f, 0,   0, 1, 0);
Enemy.x=-45;
Enemy.z=-38;
Enemy.life=100;
ammol=70;
ammorocket=5;
ammoplasma=10;
compfrag=0;
myfrag=0;
life=100;
armora=0;
Enemy.armor=0;
RocketLauncher.taken=false;
PlasmaThrower.taken=false;
weapon=Lasergun;
}

void Menu(){
			glColor3f(0,1,1);
		glPrint(20,370,"Winter Wars 2",0,4,4,4);
		glColor3f(0.55f,1,0.5f);
		glPrint(150,340,"By Megabyte Softworks(Michalbb)",0,1,1,1);
		glColor3f(1,1,0);
		glPrint(5,250,"Located somewhere in the winter, you have found a terrorist, who wants to attack",0,0.7f,0.7f,0.7f);
	    glPrint(5,240,"you. But he doesn't know, that you're from commando. Show 'em who you are!",0,0.7f,0.7f,0.7f); 
	
	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
		glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0,230);
	glVertex2f(640,230);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(300,175);
	glVertex2f(400,175);
	glVertex2f(400,125);
	glVertex2f(300,125);
	glVertex2f(300,175);
	glEnd();
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glColor3f(0.5f,0.7f,0);
	glPrint(40,200,"Enter the number of frags. Use up and down arrow key.",0,1,1,1);
	glColor3f(1,0.5f,1);
glPrint(295,125,frags,0,3,3,3);
if(GetKeyState(VK_UP) & 0x80  && addone){
	addone=false;
	fragsall+=1;
}
if(GetKeyState(VK_DOWN) & 0x80  && subone){
	subone=false;
	fragsall-=1;
}
if(!GetKeyState(VK_UP))addone=true;
if(!GetKeyState(VK_DOWN))subone=true;
	if(fragsall<1)fragsall=1;
	glColor3f(1,1,1);
glPrint(150,100,"Press SPACEBAR when you're done",0,1,1,1);
if(GetKeyState(' ') & 0x80){
	menu=false; glLineWidth(1);
	PlayMidi("Data/Music/Xmas.mid");
}
}


void RenderScene() 
{
gcvt(fragsall,3,frags);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(menu && win==!1){
Menu();
	}


	if(!menu){
	g_Camera.Look();
camerax=g_Camera.m_vPosition.x;
cameraz=g_Camera.m_vPosition.z;
CreateWorld();
gcvt(myfrag,3,myfrags);
gcvt(compfrag,3,compfrags);
glPrint(60,460,myfrags,0,1,1,1);
glPrint(115,440,compfrags,0,1,1,1);


Drawthem();
DisplayLasers();
DisplayRockets();
DisplayPlasmas();
EnemyMain();
glPrint(10,460,"You:",0,1,1,1);
glPrint(10,440,"Computer:",0,1,1,1);
Ortho();


Shoot(0.55f,5.0f,2.0f);

DoWhatYouReSupposedToDo();
		if(fragsall==compfrag){menu=true; win=2;ReInit();}
	if(fragsall==myfrag){menu=true;win=1; ReInit();}
if(!keys[VK_LBUTTON]){inc=0.0f;timer=0.1f;}
	}

	if(win==1){
		glPrint(100,300,"You have won it! Yeah!",0,2,2,2);
		glPrint(100,250,"Press Esc to return to menu.",0,1,1,1);
		glPrint(100,200,"Thank you for playing!",0,1,1,1);
		glPrint(10,150,"Don't forget to read readme!",0,1.9f,1.9f,1.9f);
	}
	else if(win==2){

		glPrint(100,300,"You have lost it!",0,2,2,2);
		glPrint(100,250,"Press Esc to return to menu.",0,1,1,1);
		glPrint(10,200,"How could it happen?",0,1,1,1);
		glPrint(10,150,"TRY AGAIN",0,2,2,2);
	}
	SwapBuffers(g_hDC);	
rot+=1.5f;
timer+=inc;

}

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (uMsg)
	{ 
    case WM_SIZE:								
		if(!g_bFullScreen)								
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);					// Get the window rectangle
		}

        break; 
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_KEYDOWN:

		switch(wParam) {								
			case VK_ESCAPE:	
				if(menu && win==0){
				PostQuitMessage(0);	
				} else menu=true; ReInit(); PlayMidi("Data/Music/Menu.mid");
				if(win==1){
					win=0;
					ReInit();
				}
				if(win==2){win=0; ReInit();}
				break;


		}
		break;

		case WM_LBUTTONDOWN:{keys[VK_LBUTTON] = TRUE;return 0;}break;
		case WM_LBUTTONUP:{keys[VK_LBUTTON] = FALSE;return 0;}break;
		case WM_RBUTTONDOWN:{keys[VK_RBUTTON] = TRUE;return 0;}break;
		case WM_RBUTTONUP:{keys[VK_RBUTTON] = FALSE;return 0;}break;
	

break;

    case WM_CLOSE:										// If the window is being closes
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}
