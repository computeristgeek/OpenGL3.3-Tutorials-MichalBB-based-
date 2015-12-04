//*********************************** Main_draw.cpp *******************************************
// 
//							   Main game drawing functions.
//
//*********************************** Main_draw.cpp *******************************************

#include "Main.h"

////////////////////////////////////////// INIT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function initializes whole application
//
////////////////////////////////////////// INIT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLuint textures[11]; // Create textures
Soldier g_soldier; // Create soldier
FSOUND_SAMPLE *sounds[5]; // Creaate sounds
vector<Machiner> machiners; // Create machine gunners
vector<Robot> robots; // Create robots
vector<Can> cans; // Create cans
vector<CVector2> health; // Create healths' objects
vector<CVector2> bullets; // Create bullets' objects
vector<CVector2> rockets; // Create rockets' objects
vector<CVector2> bolts; // Create healths' objects

GLint enleft=100; // Enemies left
GLboolean win=false;

GLvoid Init()
{
	RECT rect; // Rectangle variable
	GetClientRect(g_Wnd,&rect); // Get window rectangle
	InitOpenGL(rect.right,rect.bottom); // Set OpenGL screen to client window
	BuildFont(); // Build font

	char lastdir[355]={0};
	GetCurrentDirectory(355,lastdir);
	char newdir[400]={0};
	strcat(newdir,lastdir);
	strcat(newdir,"/Data/Textures");
	SetCurrentDirectory(newdir);

	CreateTexture(&textures[0],"Soldier_CG.bmp"); // Load all textures
	CreateTexture(&textures[1],"Soldier_RL.bmp");
	CreateTexture(&textures[2],"Soldier_EG.bmp");
	CreateTexture(&textures[3],"Rocket.bmp");
	CreateTexture(&textures[4],"Machiner.bmp");
	CreateTexture(&textures[5],"Robot.bmp");
	CreateTexture(&textures[6],"Health.bmp");
	CreateTexture(&textures[7],"Bullets.bmp");
	CreateTexture(&textures[8],"Rockets.bmp");
	CreateTexture(&textures[9],"Bolts.bmp");
	CreateTexture(&textures[10],"TheCan.bmp");

	SetCurrentDirectory(lastdir);

	FSOUND_Init (44100, 32, FSOUND_INIT_GLOBALFOCUS); // Load all sounds
	sounds[0]=FSOUND_Sample_Load (FSOUND_FREE,"Data/Sounds/M4.wav",0,0);
	sounds[1]=FSOUND_Sample_Load (FSOUND_FREE,"Data/Sounds/Rocket.wav",0,0);
	sounds[2]=FSOUND_Sample_Load (FSOUND_FREE,"Data/Sounds/Electro.wav",0,0);
	sounds[3]=FSOUND_Sample_Load (FSOUND_FREE,"Data/Sounds/Chaingun.wav",0,0);
	sounds[4]=FSOUND_Sample_Load (FSOUND_FREE,"Data/Sounds/Bonus.wav",0,0);
	PlayIntro();
}

////////////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function draws everything to window.
//
////////////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid RenderScene() 
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset the matrix
	gluLookAt(0,0,50,	0,0,0,	0,1,0); // Look
	g_soldier.Draw(); // Render soldier

	for(GLint s=0;s<machiners.size();s++)
	{
		machiners[s].Draw(); // Render machine gunner
		if(machiners[s].life<=0) // If his life is less than 0
		{
				GLint random2=rand()%11; // Create random object
				if(random2<5)
				bullets.push_back(machiners[s].mvPos);
				else if(random2>4 && random2<8)
				rockets.push_back(machiners[s].mvPos);
				else bolts.push_back(machiners[s].mvPos);

				GLint luck=rand()%3;
				if(luck==0)
				{
					GLint randobj2=rand()%2;
					if(randobj2)bolts.push_back(CVector2(machiners[s].mvPos.x+1,machiners[s].mvPos.y));
					else rockets.push_back(CVector2(machiners[s].mvPos.x+1,machiners[s].mvPos.y));
				}
			DeleteFromVector(machiners,s); // Delete
			enleft--; // Subtract from enemies left
		}
	}

	for(s=0;s<robots.size();s++)
	{
		robots[s].Draw(); // Render robot
		if(robots[s].life<=0)
		{
			health.push_back(robots[s].mvPos); // Create health

			DeleteFromVector(robots,s); // Delete
			enleft--; // Subtract from enemies left
		}
	}

	for(s=0;s<cans.size();s++)
	{
		cans[s].Draw(); // Render can
		if(cans[s].life<=0)
		{
			health.push_back(cans[s].mvPos); // Create health
			DeleteFromVector(cans,s); // Delete
			enleft--; // Subtract from enemies left
		}
	}

	glBindTexture(GL_TEXTURE_2D,textures[6]);
	glColor3f(1,1,1);
	for(s=0;s<health.size();s++)
	{
		// Render health
		glPushMatrix();
		glTranslatef(health[s].x,health[s].y,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(-0.5f,0.5f);
		glTexCoord2f(1,1); glVertex2f(0.5f,0.5f);
		glTexCoord2f(1,0); glVertex2f(0.5f,-0.5f);
		glTexCoord2f(0,0); glVertex2f(-0.5f,-0.5f);
		glEnd();
		glPopMatrix();
		if(CBCollide(CVector2(0.5f,0.5f),CVector2(2,2),health[s],g_soldier.mvPos) && g_soldier.life<200)
		{
			g_soldier.life+=20;
			DeleteFromVector(health,s);
			FSOUND_PlaySound(FSOUND_FREE,sounds[4]);
		}
	}

	glBindTexture(GL_TEXTURE_2D,textures[7]);
	for(s=0;s<bullets.size();s++)
	{
		// Render bullets
		glPushMatrix();
		glTranslatef(bullets[s].x,bullets[s].y,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(-0.5f,0.5f);
		glTexCoord2f(1,1); glVertex2f(0.5f,0.5f);
		glTexCoord2f(1,0); glVertex2f(0.5f,-0.5f);
		glTexCoord2f(0,0); glVertex2f(-0.5f,-0.5f);
		glEnd();
		glPopMatrix();
		if(CBCollide(CVector2(0.5f,0.5f),CVector2(2,2),bullets[s],g_soldier.mvPos))
		{
			g_soldier.ammo[0]+=50;
			DeleteFromVector(bullets,s);
			FSOUND_PlaySound(FSOUND_FREE,sounds[4]);
		}
	}
	glBindTexture(GL_TEXTURE_2D,textures[8]);
	for(s=0;s<rockets.size();s++)
	{
		// Render rockets
		glPushMatrix();
		glTranslatef(rockets[s].x,rockets[s].y,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(-0.5f,0.5f);
		glTexCoord2f(1,1); glVertex2f(0.5f,0.5f);
		glTexCoord2f(1,0); glVertex2f(0.5f,-0.5f);
		glTexCoord2f(0,0); glVertex2f(-0.5f,-0.5f);
		glEnd();
		glPopMatrix();
		if(CBCollide(CVector2(0.5f,0.5f),CVector2(2,2),rockets[s],g_soldier.mvPos))
		{
			g_soldier.ammo[1]+=8;
			DeleteFromVector(rockets,s);
			FSOUND_PlaySound(FSOUND_FREE,sounds[4]);
		}
	}
	glBindTexture(GL_TEXTURE_2D,textures[9]);
	for(s=0;s<bolts.size();s++)
	{
		// Render bolts
		glPushMatrix();
		glTranslatef(bolts[s].x,bolts[s].y,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(-0.5f,0.5f);
		glTexCoord2f(1,1); glVertex2f(0.5f,0.5f);
		glTexCoord2f(1,0); glVertex2f(0.5f,-0.5f);
		glTexCoord2f(0,0); glVertex2f(-0.5f,-0.5f);
		glEnd();
		glPopMatrix();
		if(CBCollide(CVector2(0.5f,0.5f),CVector2(2,2),bolts[s],g_soldier.mvPos))
		{
			g_soldier.ammo[2]+=27;
			DeleteFromVector(bolts,s);
			FSOUND_PlaySound(FSOUND_FREE,sounds[4]);
		}
	}
	// PrGLint enemies left text
	PrintText(CVector2(620,580),1,Rgb(1,1,1),0,NOTHING,"Enemies left: %d",enleft);
	g_soldier.DisplayInfo(); // Display soldier's info
	AddEnemy(); // Add enemies
	if(onekey(VK_ESCAPE))quitapp=true;
	SwapBuffers(g_DC); // Swap buffers
	if(enleft==0){win=true;PlayOutro();}
	if(g_soldier.life<0){win=false;PlayOutro();}
}

////////////////////////////////////////// WNDPROC \\\\\\\\\\\\\\\\=\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This functions works with window messages.
//
////////////////////////////////////////// WNDPROC \\\\\\\\\\\\\\\\=\\\\\\\\\\\\\\\\\\\\\\\\\\*

LRESULT CALLBACK WndProc(GLuint hWnd,GLuint uMsg, WPARAM wParam, LPARAM lParam)
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

		default:									
			lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
			break; 
    } 
    return lRet;	// Return by default
}

