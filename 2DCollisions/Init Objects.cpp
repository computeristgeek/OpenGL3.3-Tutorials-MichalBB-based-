#include "main.h"
#include "Weapons.h"
Enemy Pajco[CRAFT];
Ammo plasma[PLASMA];
Ammo bullet[BULLET];
float ydam;
float mylife=200.0f;
int count;
bool display=FALSE;

void Ryby(){
	for(int ryby=0;ryby<CRAFT;ryby++){
	Pajco[ryby].x=float(rand()%200)*0.05f;
	Pajco[ryby].y=float(rand()%50)*0.05f;
	Pajco[ryby].life=5.0f;
	Pajco[ryby].one=TRUE;
		Pajco[ryby].timerexpl=0.0f;
		Pajco[ryby].count=TRUE;
	}
}
void Craft(){
	for(int joj=0;joj<CRAFT;joj++){
if(Pajco[joj].life>0.0f){
DrawEnemy(Pajco[joj].x,Pajco[joj].y);
Pajco[joj].y-=0.002f;
	}
else{
	Pajco[joj].x=-50.0f;
	if(Pajco[joj].count){
		Pajco[joj].count=FALSE;
	count+=1;
	}
	}


	}
}

void DrawAmmo(){
		for(int loop=0;loop<ammop;loop++){
		float xa=plasma[loop].x;
		float ya=plasma[loop].y;
		if(plasma[loop].active){
		glPushMatrix();
glTranslatef(xa,ya,0);
glBegin(GL_QUADS);
			glColor3ub(255,255,255);
			glVertex2f(-0.02f,0.02f);
			glVertex2f(0.02f,0.02f);
			glVertex2f(0.02f,-0.02f);
			glVertex2f(-0.02f,-0.02f);
			glEnd();
glPopMatrix();

		}
		
		if(plasma[loop].one){
plasma[loop].one=FALSE;
plasma[loop].x=x;
plasma[loop].y=y;
plasma[loop].xinc=(float)sin(rotation*piover180) * 0.05f;
plasma[loop].yinc=(float)cos(rotation*piover180) * 0.05f;
		}
plasma[loop].x-=plasma[loop].xinc*2;
plasma[loop].y+=plasma[loop].yinc*2;
for(int ryby=0;ryby<CRAFT;ryby++){
	//Collision
if(plasma[loop].x>Pajco[ryby].x-0.5f && plasma[loop].x<Pajco[ryby].x+0.5f && plasma[loop].y>Pajco[ryby].y-0.5f && plasma[loop].y<Pajco[ryby].y+0.5f){plasma[loop].x=65.0f; Pajco[ryby].life-=plasma[loop].damage; plasma[loop].active=FALSE;}
	}
		}
}

void InitAmmo(){
	for(int loop=0;loop<PLASMA;loop++){
		plasma[loop].one=TRUE;
		plasma[loop].active=TRUE;
	
			plasma[loop].damage=0.2f;		
	}
	for(loop=0;loop<BULLET;loop++){
		bullet[loop].active=TRUE;
        bullet[loop].one=TRUE;
		bullet[loop].sound=TRUE;
		bullet[loop].damage=0.2f;
	
		
	}
}

float timer,inc;
void PlayDelayedSound(float time){
	if(GetKeyState(' ') & 0x80){
		inc=0.1f;
	}
	if(timer==0.1f){
 PlaySound("Data/Fire1.wav",NULL,SND_ASYNC);

	}
	if(timer>time){
		timer=0.0f;
	}
}

void KeyInput(){
	if(GetKeyState(VK_LEFT) & 0x80){
		rotation+=3.5f;
	}
	if(GetKeyState(VK_RIGHT) & 0x80){
		rotation-=3.5f;
	}
		if(GetKeyState(VK_UP) & 0x80){
	x-=xinc;
	y+=yinc;
	hrot=TRUE;
	}
		else{
			hrot=FALSE;
		}
				if(GetKeyState(VK_DOWN) & 0x80){
	x+=xinc;
	y-=yinc;
	}
	if(GetKeyState(' ') & 0x80){ammop+=1;PlayDelayedSound(1.1f);}
	else{inc=0.0f;timer=0.0f;}


}
float rotob;
Object damage[3];

void initd(){
	for(int dam=0;dam<4;dam++){
	damage[dam].active=TRUE;
	damage[dam].sound=TRUE;
	}
		damage[0].x=5.0f;
	damage[0].y=3.0f;
		damage[1].x=20.0f;
	damage[1].y=-15.0f;
		damage[2].x=35.0f;
	damage[2].y=0.0f;
		damage[3].x=-15.0f;
	damage[3].y=20.0f;

}
void Damagea(float xaa, float yaa,int num){

	if(damage[num].active){
	glPushMatrix();
	glTranslatef(xaa,yaa,0);
	glRotatef(rotob,0,1,0);
	glBindTexture(GL_TEXTURE_2D, g_Texture[2]);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	glTexCoord2f(0,1); glVertex2f(-0.2f,0.2f);
	glTexCoord2f(1,1); glVertex2f(0.2f,0.2f);
	glTexCoord2f(1,0); glVertex2f(0.2f,-0.2f);
	glTexCoord2f(0,0); glVertex2f(-0.2f,-0.2f);
	glEnd();
	glPopMatrix();
	}
		rotob+=1.5f;
}

void TestCollision(){
			for(int dam=0;dam<4;dam++){
	if(x-0.4f<damage[dam].x && x+0.4f>damage[dam].x && y-0.4f<damage[dam].y && y+0.4f>damage[dam].y){
	damage[dam].active=FALSE;
	for(int ammo=0;ammo<PLASMA;ammo++){
		plasma[ammo].damage=0.8f;
ydam=470;
display=TRUE;
damage[dam].x=-500.0f;

		if(damage[dam].sound){damage[dam].sound=FALSE;PlaySound("Data/Damage.wav",NULL,SND_ASYNC);}
	}
	}
	}
ydam-=0.3f;
}

void Vyvolaj(){
	Damagea(damage[1].x,damage[1].y,1);
	Damagea(damage[2].x,damage[2].y,2);
	Damagea(damage[3].x,damage[3].y,3);
		Damagea(damage[0].x,damage[0].y,0);
}

void Ortho(){
	
	glDisable(GL_DEPTH_TEST);					
	glMatrixMode(GL_PROJECTION);				
	glPushMatrix();									
	glLoadIdentity();								
	glOrtho(0,640,0,480,-1,1);							
	glMatrixMode(GL_MODELVIEW);							
	glPushMatrix();										
	glLoadIdentity();
	if(display){
glBegin(GL_QUADS);
glVertex2f(5,ydam);
glVertex2f(20,ydam);
glVertex2f(20,20);
glVertex2f(5,20);								
glEnd();
	}
	glBegin(GL_QUADS);
	glColor3f(0,1,0);
glVertex2f(5,10);
glVertex2f(mylife*3,10);
glVertex2f(mylife*3,5);
glVertex2f(5,5);								
glEnd();
	glMatrixMode(GL_PROJECTION);					
	glPopMatrix();										
	glMatrixMode(GL_MODELVIEW);							
	glPopMatrix();										
	glEnable(GL_DEPTH_TEST);

	if(ydam<=0){
		display=FALSE;

		for(int ammo=0;ammo<PLASMA;ammo++){
			plasma[ammo].damage=0.2f;
		}
	}
}

void EnemyShot(){
	for(int loop=0;loop<BULLET;loop++){
		for(int loo=0;loo<CRAFT;loo++){
	if(bullet[loop].one){
		bullet[loop].one=FALSE;
	bullet[loop].positive=rand()%2;
		bullet[loop].xinc=float(rand()%10)*0.0002f;
		bullet[loop].yinc=float(rand()%10)*0.0002f;
		if(Pajco[loop].life>0){
		bullet[loop].y=Pajco[rand()%20].y;
			bullet[loop].x=Pajco[rand()%20].x;
		}
	}
	if(bullet[loop].active){
		glPushMatrix();
		glTranslatef(bullet[loop].x,bullet[loop].y,0);
		glBegin(GL_QUADS);
					glVertex2f(-0.02f,0.02f);
			glVertex2f(0.02f,0.02f);
			glVertex2f(0.02f,-0.02f);
			glVertex2f(-0.02f,-0.02f);
			glEnd();
			glPopMatrix();
			if(bullet[loop].positive){
			bullet[loop].x-=bullet[loop].xinc;
			bullet[loop].y-=bullet[loop].yinc;}
			}
		if(!bullet[loop].positive){
			bullet[loop].x-=bullet[loop].xinc;
			bullet[loop].y+=bullet[loop].yinc;}
		
	if(x-0.2f<bullet[loop].x && x+0.2f>bullet[loop].x && y-0.2f<bullet[loop].y && y+0.2f>bullet[loop].y && bullet[loop].sound && bullet[loop].active){
bullet[loop].sound=FALSE; PlaySound("Data/Laser.wav",NULL,SND_ASYNC);bullet[loop].active=FALSE; bullet[loop].x=-100.0f; 
mylife-=5;}
	
	if(bullet[loop].x+0.5f<x){bullet[loop].sound=TRUE;}
			if(bullet[loop].x<-10.0f){bullet[loop].x=Pajco[loo].x;bullet[loop].y=Pajco[loo].y;  bullet[loop].active=TRUE;}
			
}
	}
}