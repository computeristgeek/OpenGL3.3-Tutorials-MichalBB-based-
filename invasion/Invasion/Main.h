//					  MEGABYTE SOFTWORKS PROGRAMMING BASECODE
//
//     Author: Michalbb ( michalbb@centrum.sk )
//     License: You may use this basecode in non-commercial games
//
//---------------------------------------------------------------------------------------------

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <math.h>
#include <vector> // Vector header file

using namespace std;

#include "Ammo.h"
#include "Camera.h"
#include "Enemy.h"
#include "Model.h"

#define PI		3.14159265358979323846f // PI number for angles and circles
#define MAXMODEL 5 // Max number for models
#define MAXTEXTURES 5

extern HWND g_Wnd;		  // Our global window variable used for lots of functions
extern HINSTANCE g_Inst; // Global application instance variable
extern HDC g_DC;		// Global device context variable
extern HGLRC g_RC;      // Global rendering context variable

extern Model *pModel[MAXMODEL];
extern int allkills;
extern int game,high;

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
// World displaying
	void DrawWorld();
// This loads all textures
	void LoadAllTextures();
// Our texture array
	extern UINT textures[MAXTEXTURES];
// Create Texture function
	void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);
// Draws any CAmmo
	void DrawAmmo(CAmmo &ammo);
// Draws any CEnemy
	void DrawEnemy(CEnemy &enemy);
// These are drawing functions
	void DrawRedLaser();
// This function deletes ammo
	void DeleteAmmo(CAmmo &ammo, int num);
// This function returns distance betwwen two points
	float Distance(CVector3 vPoint1, CVector3 vPoint2);
// This function checks for ammo out of the world
	bool AmmoOutOfWorld(Ammo &ammo);
// This functions checks for camera in the world
	bool CameraInWorld(CCamera &camera);
// This loads all models into memory
	void LoadModels();
// This is really great random function
	float grand(int num);
// This draws any explosion
	void DrawExplosions(CExplo &explo);
// Text printing
	GLvoid glPrint(GLint x, GLint y,int set,float scalex, float scaley, float scalez,float colx, float coly, float colz,const char *string, ...);
// Building font
	void BuildFont(GLvoid);
// This draws everything in ortho mode
	void OrthoMode();
// This draws missile
	void DrawMissile();
// This changes matrix mode
	void Start2D(),End2D();
// This draws menu
	void RenderMenu();
// This initializes menu
	void InitMenu();
// This is function that calls in the end of game
	void Result();
// Highscore reading
	void ReadHS();
// Shooting function
	void Shoot();
// Message box
	bool MSGBox(char*question,char*title);
// Game reset
	void ResetGame();
// Midi music play
	BOOL PlayMidi(char *sFileName);
// Font loading
	void LoadFont();




