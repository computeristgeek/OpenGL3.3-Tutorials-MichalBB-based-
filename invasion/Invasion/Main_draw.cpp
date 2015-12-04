//*********************************** Main_draw.cpp *******************************************
// 
//							   Main game drawing functions.
//
//*********************************** Main_draw.cpp *******************************************

#include "Main.h"

CCamera Main;
CEnemy martian;
CAmmo Mlasers,Glasers, missiles;
CPart mp;
CMb missilebase;

int weapon=0;
int allkills=0;

/////////////////////////////////// DRAWING FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//			These functions are used by ammo and enemy function pointers.
//
/////////////////////////////////// DRAWING FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawMartian() // This draws martian
{
	pModel[0]->draw();
}

void DrawLaser(float red, float green, float blue) // This function draws laser
{
	glColor3f(red,green,blue);
	glBegin(GL_QUADS); 
	glVertex3f(-0.02f,0.02f,0.18f);
	glVertex3f(0.02f,0.02f,0.18f);
	glVertex3f(0.02f,-0.02f,0.18f);
	glVertex3f(-0.02f,-0.02f,0.18f);
//*********************************************
	glVertex3f(-0.02f,0.02f,-0.18f);
	glVertex3f(0.02f,0.02f,-0.18f);
	glVertex3f(0.02f,-0.02f,-0.18f);
	glVertex3f(-0.02f,0.02f,-0.18f);
//*********************************************
	glVertex3f(-0.02f,0.02f,0.18f);
	glVertex3f(-0.02f,0.02f,-0.18f);
	glVertex3f(-0.02f,-0.02f,-0.18f);
	glVertex3f(-0.02f,-0.02f,0.18f);
//*********************************************
	glVertex3f(0.02f,0.02f,0.18f);
	glVertex3f(0.02f,0.02f,-0.18f);
	glVertex3f(0.02f,-0.02f,-0.18f);
	glVertex3f(0.02f,-0.02f,0.18f);
//********************************************
	glVertex3f(-0.02f,0.02f,0.18f);
	glVertex3f(-0.02f,0.02f,-0.18f);
	glVertex3f(0.02f,0.02f,-0.18f);
	glVertex3f(0.02f,0.02f,0.18f);
//*********************************************
	glVertex3f(-0.02f,-0.02f,0.18f);
	glVertex3f(-0.02f,-0.02f,-0.18f);
	glVertex3f(0.02f,-0.02f,-0.18f);
	glVertex3f(0.02f,-0.02f,0.18f);
	glEnd();
	glColor3f(1,1,1);
}
void DrawRedLaser(){ DrawLaser(1,0,0);}
void DrawGreenLaser(){DrawLaser(0,1,0);}
void DrawMissile(){pModel[1]->draw();}

////////////////////////////////////////// STAMSG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								  Status message function.
//
////////////////////////////////////////// STAMSG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void StatMSG(char*msg)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPrint(200,100,1,2,2,2,1,1,1,msg);
	glPrint(130,300,0,1,1,1,0,0.5f,1,"----- Initializing application... Please wait.  -----");

SwapBuffers(g_DC);

}

////////////////////////////////////////// INIT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function initializes whole application
//
////////////////////////////////////////// INIT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
	RECT rect;
	GetClientRect(g_Wnd,&rect);
	InitOpenGL(rect.right,rect.bottom);
	StatMSG("Loading textures...");
	LoadAllTextures();
	Main.PositionCamera(0,3,0,	0,3,-0.5f,	0,1,0);
	StatMSG("Loading music...");
	LoadModels();
	InitMenu();
	ReadHS();
	PlayMidi("Data/Music/Main.mid");
}

///////////////////////////////////////// SHOOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								Main shooting function.
//
///////////////////////////////////////// SHOOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Shoot()
{
	static float time;
	if(GetKeyState(VK_LBUTTON) & 0x80)
	{
		switch(weapon)
		{
		case 0:
			Glasers.push_back(Ammo(Main.m_vPosition,(Main.m_vView-Main.m_vPosition)*3,4,DrawGreenLaser,Main.anX,Main.anY)); if(time>mt){time=0;PlaySound("Data/Sounds/Laser.wav",NULL,SND_ASYNC); }
			break;
		case 1: 
			if(missile>0 && time>mt){time=0;missile-=1;missiles.push_back(Ammo(Main.m_vPosition,(Main.m_vView-Main.m_vPosition)*2,100,DrawMissile,-Main.anX,Main.anY)); PlaySound("Data/Sounds/Missile.wav",NULL,SND_ASYNC);}
			break;
		case 2:
			if(missilebase.size()==0 && mbs>0){mbs-=1;missilebase.push_back(MissileBase(CVector3(Main.m_vPosition.x,Main.m_vPosition.y-3,Main.m_vPosition.z))); PlaySound("Data/Sounds/Mbs.wav",NULL,SND_ASYNC);}
			break;
		}
	}
	time+=0.01f;
}

/////////////////////////////////// CREATENEWENENMIES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function creates new enemies in time ms.
//
/////////////////////////////////// CREATENEWENENMIES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateNewEnemies(float ms)
{
	static float last=GetTickCount();
	float current=GetTickCount();
	if(last+ms<current)
	{
		last=GetTickCount();
		int where=rand()%4;
		switch(where)
		{
		case 0: // Front
			martian.push_back(Enemy(CVector3(0,200,-400),20,DrawMartian));
			break;
		case 1: // Back
			martian.push_back(Enemy(CVector3(0,200,400),20,DrawMartian));
			break;
		case 2: // Left
			martian.push_back(Enemy(CVector3(-400,200,0),20,DrawMartian));
			break;
		case 3: // Right
			martian.push_back(Enemy(CVector3(400,200,0),20,DrawMartian));
			break;
		}
	}
}

////////////////////////////////////// MAKEMISSILE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						    This function makes ammo missile.
//
////////////////////////////////////// MAKEMISSILE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void MakeMissile(CAmmo &ammo, const CEnemy &enemy)
{
	for(int v=0;v<(int)ammo.size();v++)
	{
		if(ammo[v].follow==-1 && enemy.size()!=0)ammo[v].follow=rand()%(int)(enemy.size());
		if(ammo[v].follow>(int)enemy.size() && enemy.size()!=0)ammo[v].follow=rand()%(int)(enemy.size());
	}
}

//////////////////////////////////// AMMOPATHMAKER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						    This function creates particles behind ammo.
//
//////////////////////////////////// AMMOPATHMAKER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void AmmoPathMaker(CAmmo &ammo,CPart &part)
{
	for(int l=0;l<(int)ammo.size();l++)
	{
		part.push_back(Particle(ammo[l].pos,
		CVector3(-ammo[l].inc.x/4+grand(200)*0.0001f,
		-ammo[l].inc.y/4+grand(200)*0.0001f,
		-ammo[l].inc.z/4+grand(200)*0.0001f)));
	}
}	


float enemytime=1000;

////////////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function draws everything to window.
//
////////////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
	if(life>100)life=100;
	enemytime=1000-allkills*2;
	if(weapon>2)weapon=2; if(weapon<0)weapon=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear the screen and depth buffer
	glLoadIdentity();									// Reset the matrix
	Main.Look(); // Look with our camera

	DrawWorld(); // Draw the world
	DrawEnemy(martian);
	AmmoPathMaker(missiles,mp);
	DrawAmmo(missiles);
	glDisable(GL_TEXTURE_2D);
	DrawAmmo(Mlasers);
	DrawAmmo(Glasers);
	DrawParticles(mp);
	DrawMB(missilebase);
	DrawAllObjects();
	glEnable(GL_TEXTURE_2D);
	Shoot();
	glPrint(530,550,0,1,1,1,0,1,0,"Total units killed: %d",allkills);
	DrawExplosions(mart);
	OrthoMode();
	SwapBuffers(g_DC);	// Swap buffers

	if(enemytime<400)enemytime=400;

	CreateNewEnemies(enemytime);

	MakeMissile(missiles,martian);
	CheckAmmoAndCamera(Main,Mlasers);
	if(life<=0)game=2;

	if(GetKeyState(VK_ESCAPE) & 0x80)
	{
		if(MSGBox("Are you sure you want to cancel game?","Quit game"))
		{
			game=0;ResetGame();
		}
	}

	Main.Update();		// Update camera
}

////////////////////////////////////////// WNDPROC \\\\\\\\\\\\\\\\=\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This functions works with window messages.
//
////////////////////////////////////////// WNDPROC \\\\\\\\\\\\\\\\=\\\\\\\\\\\\\\\\\\\\\\\\\\*

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (uMsg)
	{ 
		case WM_DESTROY:
			QuitApplication();
			break;

		case WM_PAINT:									
			BeginPaint(g_Wnd, &ps);							
			EndPaint(g_Wnd, &ps);					
			break;

		case 522:
		{
			short delta=HIWORD(wParam);
			if(delta>0)weapon+=1;
			else weapon-=1;
		}


		default:									
			lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
			break; 
    } 
    return lRet;										// Return by default
}

