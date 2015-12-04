#include "Main.h"


bool keys[256],kp[256];

void SetKeys()
{
	for(int l=0;l<256;l++)
	{
		if(GetKeyState(l) & 0x80)keys[l]=true; else keys[l]=false;
	}
}

bool onekey(int keyid)
{
	if(keys[keyid] && !kp[keyid]){kp[keyid]=true; return true;}
	if(!keys[keyid]){kp[keyid]=false; return false;}
	
	return false;
}

struct Option{
	int x,y;
	LPSTR caption;
};



struct Menu
{
	Option* options;

	int all;
	int active;
	
};


Menu main;

Option optijons[]={
	{250,400,"Start Game"},
	{335,350,"Quit"}
};

void InitMenu()
{
	main.all=2;
	main.active=0;
	main.options=optijons;

}

////////////////////////////////////////// MSGBOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									A MeSsaGe box function.
//
////////////////////////////////////////// MSGBOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool MSGBox(char*question,char*title)
{

	MSG msg;
	int yesno=1;

	while(1)
	{
		SetKeys();
										
											// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
			{PostQuitMessage(0);break;}
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 

	Start2D();
glBindTexture(GL_TEXTURE_2D,textures[0]);
glBegin(GL_QUADS);
glColor3f(1,1,1);
glTexCoord2f(0,1); glVertex2f(200,450);
glTexCoord2f(1,1); glVertex2f(600,450);
glTexCoord2f(1,0); glVertex2f(600,150);
glTexCoord2f(0,0); glVertex2f(200,150);
glEnd();
	End2D();

glPrint(210,300,0,1,1,1,0,1,0,question);
glPrint(220,400,1,2,2,2,1,1,0,title);
if(yesno==0){
glPrint(210,180,0,2,2,2,1,1,1,"Yes");
}
else{
glPrint(550,180,0,2,2,2,1,1,1,"No");
}

if(onekey(VK_LEFT))yesno--;
if(onekey(VK_RIGHT))yesno++;
if(yesno>1)yesno=1;
if(yesno<0)yesno=0;
if(onekey(VK_RETURN))
{
	if(yesno==0)return true;
	else return false;
}
		SwapBuffers(g_DC);	
		}
	}

return false;
} 

int game=0;

void MakeAction(int num)
{
	if(num==0){game=1;}
	if(num==1){if(MSGBox("Are you sure you want to quit?","Confirm."))
	{PostQuitMessage(0);}
	}
}



void RenderMenu()
{
	static float roll;

	SetKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Start2D();
	glBindTexture(GL_TEXTURE_2D, textures[1]);	
	glBegin(GL_QUADS);
		glColor3f(1,1,1);
		glTexCoord2f(0,roll+1); glVertex2f(0,600);
		glTexCoord2f(1,roll+1); glVertex2f(800,600);
		glTexCoord2f(1,roll); glVertex2f(800,0);
		glTexCoord2f(0,roll); glVertex2f(0,0);
	glEnd();

	End2D();

	glPrint(225,500,0,4,4,4,0,0.5f,1,"Invasion");

	for(int cap=0;cap<main.all;cap++)
	{

		if(cap==main.active){
		glPrint(main.options[cap].x,main.options[cap].y,0,3,3,3,1,0,0,main.options[cap].caption);
		}
		else{
		glPrint(main.options[cap].x,main.options[cap].y,0,3,3,3,0,0.5f,0,main.options[cap].caption);
	}
}

	glPrint(50,200,0,2,2,2,1,0,1,"Actual highscore is: %d",high);

	SwapBuffers(g_DC);	

	if(onekey(VK_UP)){main.active--;PlaySound("Data/Sounds/Menu_move.wav",NULL,SND_ASYNC);}
	if(onekey(VK_DOWN)){main.active++;PlaySound("Data/Sounds/Menu_move.wav",NULL,SND_ASYNC);}
	if(main.active>main.all-1)main.active=0;
	if(main.active<0)main.active=main.all-1;
	if(onekey(VK_RETURN)){PlaySound("Data/Sounds/Menu_select.wav",NULL,SND_ASYNC);MakeAction(main.active);}
	
roll+=0.01f;
}

int high;

void ReadHS()
{
	FILE* fp=fopen("Data/Highscore.dat","rb");
	fread(&high,sizeof(int),1,fp);
	fclose(fp);
}

void WriteHS()
{
	if(allkills>high)
	{
		FILE* fp=fopen("Data/Highscore.dat","wb");
		fwrite(&allkills,sizeof(int),1,fp);
		fclose(fp);
		high=allkills;
	}
}


void ResetGame()
{
	allkills=0;
	martian.clear();
	mart.clear();
	Glasers.clear();
	Mlasers.clear();
	life=100;
	mbs=0;
	missile=50;
	Main.PositionCamera(0,3,0,	0,3,-0.5f,	0,1,0);
	Main.anX=Main.anY=0;
	health.clear();
	bazooka2.clear();
	missilepack.clear();
}

void Result()
{
	SetKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPrint(100,600,0,2,2,2,1,0,0,"Your score was: %d",allkills);
	if(allkills>high)
	glPrint(100,500,0,2,2,2,0,1,0,"You bet the highscore!");
	else 
	glPrint(10,500,0,1,1,1,0,1,0,"You didn't beat the highscore. Maybe next time.");
	glPrint(10,400,0,2,2,2,0,1,0,"Press Enter to return to menu.");

	if(onekey(VK_RETURN))
	{
		WriteHS();
		game=0;
		ResetGame();
	}
	SwapBuffers(g_DC);
};
