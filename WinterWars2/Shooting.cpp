#include "Weapons.h"
#include "main.h"
#include "Camera.h"
Ammo lasers[500];
Ammo rockets[100];
Ammo plasmas[250];
CCamera g_Camera;


// This function is for lasers, as many times you click left mouse button, as many lasers will be
// drawn. But if they leaves play area, they have to deactivate

void DisplayLasers(){
	for(int lsr=0;lsr<lasercount;++lsr){
float x=lasers[lsr].x,y=lasers[lsr].y,z=lasers[lsr].z;
CVector3 view;
view=g_Camera.m_vView-g_Camera.m_vPosition;
if(!lasers[lsr].active){

lasers[lsr].x=g_Camera.m_vPosition.x;
lasers[lsr].y=g_Camera.m_vPosition.y;
lasers[lsr].z=g_Camera.m_vPosition.z;
lasers[lsr].xinc=view.x;
lasers[lsr].yinc=view.y;
lasers[lsr].zinc=view.z;
lasers[lsr].active=TRUE;
}
if(lasers[lsr].active){
glPushMatrix();
glTranslatef(x,y,z);
glBindTexture(GL_TEXTURE_2D, g_Texture[6]);
glCallList(a[1]);
glPopMatrix();
lasers[lsr].x+=lasers[lsr].xinc/5;
lasers[lsr].y+=lasers[lsr].yinc/5;
lasers[lsr].z+=lasers[lsr].zinc/5;
}
if(lasers[lsr].x+1>Enemy.x && 
lasers[lsr].x-1<Enemy.x && lasers[lsr].z+1>Enemy.z &&
lasers[lsr].z-1<Enemy.z && lasers[lsr].y<3.7f && 
lasers[lsr].y>-1 && Enemy.life>0 && lasers[lsr].active)
{ 
	if(Enemy.armor>0){	
lasercount-=1;Enemy.armor-=5; Sound("Data/Sounds/Arm.wav"); lasers[lsr].active=FALSE;}
	else{
		lasercount-=1;Sound("Data/Sounds/Hurt.wav"); Enemy.life-=5; lasers[lsr].active=FALSE;
	}
}
if((lasers[lsr].x<-100 || lasers[lsr].x>100 || lasers[lsr].z<-100 || lasers[lsr].z>100) && lasers[lsr].active){lasercount-=1; lasers[lsr].active=FALSE;}
	}
}








void DisplayRockets(){
	for(int rkt=0;rkt<rocketcount;rkt++){
float x=rockets[rkt].x,y=rockets[rkt].y,z=rockets[rkt].z;
CVector3 view;
view=g_Camera.m_vView-g_Camera.m_vPosition;
if(!rockets[rkt].active){
rockets[rkt].x=g_Camera.m_vPosition.x;
rockets[rkt].y=g_Camera.m_vPosition.y;
rockets[rkt].z=g_Camera.m_vPosition.z;
rockets[rkt].xinc=view.x;
rockets[rkt].yinc=view.y;
rockets[rkt].zinc=view.z;
rockets[rkt].active=TRUE;
}

glPushMatrix();
glTranslatef(x,y,z);
if(rockets[rkt].active){
drawarocket();
glPopMatrix();
rockets[rkt].x+=rockets[rkt].xinc/10;
rockets[rkt].y+=rockets[rkt].yinc/10;
rockets[rkt].z+=rockets[rkt].zinc/10;
}
if(rockets[rkt].x+1>Enemy.x && 
rockets[rkt].x-1<Enemy.x && rockets[rkt].z+1>Enemy.z &&
rockets[rkt].z-1<Enemy.z && rockets[rkt].y<3.7f && 
rockets[rkt].y>-1 && Enemy.life>0 && rockets[rkt].active)
{
		if(Enemy.armor>0){	
rocketcount-=1;Enemy.armor-=30; Sound("Data/Sounds/Arm.wav"); rockets[rkt].active=FALSE;}
	else{
		rocketcount-=1;Sound("Data/Sounds/Hurt.wav"); Enemy.life-=30; rockets[rkt].active=FALSE;
	}
	
	 Enemy.life-=50; rockets[rkt].active=FALSE; rocketcount-=1;}
	}
}




void DisplayPlasmas(){
	for(int plm=0;plm<plasmacount;plm++){
float x=plasmas[plm].x,y=plasmas[plm].y,z=plasmas[plm].z;
CVector3 view;
view=g_Camera.m_vView-g_Camera.m_vPosition;
if(!plasmas[plm].active){
plasmas[plm].active=true;
plasmas[plm].x=g_Camera.m_vPosition.x;
plasmas[plm].y=g_Camera.m_vPosition.y;
plasmas[plm].z=g_Camera.m_vPosition.z;
plasmas[plm].xinc=view.x;
plasmas[plm].yinc=view.y;
plasmas[plm].zinc=view.z;
}
if(plasmas[plm].active){
glPushMatrix();
glTranslatef(x,y,z);
DrawPlasma();
glPopMatrix();
plasmas[plm].x+=plasmas[plm].xinc/2;
plasmas[plm].y+=plasmas[plm].yinc/2;
plasmas[plm].z+=plasmas[plm].zinc/2;
}
if(plasmas[plm].x+2>Enemy.x && 
plasmas[plm].x-2<Enemy.x && plasmas[plm].z+2>Enemy.z &&
plasmas[plm].z-2<Enemy.z && plasmas[plm].y<3.7f && 
plasmas[plm].y>-1 && Enemy.life>0 && plasmas[plm].active)
{
		if(Enemy.armor>0){	
plasmacount-=1;Enemy.armor-=12; Sound("Data/Sounds/Arm.wav"); plasmas[plm].active=FALSE;}
	else{
		plasmacount-=1;Sound("Data/Sounds/Hurt.wav"); Enemy.life-=25; plasmas[plm].active=FALSE;
			}
	
		}
	}
}



void Shoot(float lasertime,float rockettime, float plasmatime){
	
	if(keys[VK_LBUTTON] && ammol>0 && timer==0.1f && weapon==Lasergun){
	inc=0.1f;
	Sound("Data/Sounds/Laser.wav");
	ammol-=1;
lasercount+=1;

}

	if(keys[VK_LBUTTON] && ammorocket>0 && timer==0.1f && weapon==RocketLNCH){
	inc=0.1f;
	Sound("Data/Sounds/Rocket.wav");
	ammorocket-=1;
rocketcount+=1;
}

	if(keys[VK_LBUTTON] && ammoplasma>0 && timer==0.1f && weapon==Plasmathrow){
	inc=0.1f;
	Sound("Data/Sounds/Plasma.wav");
	ammoplasma-=1;
plasmacount+=1;
}
if(timer>lasertime && weapon==Lasergun){
	timer=0.0f;
}
if(timer>rockettime && weapon==RocketLNCH){
	timer=0.0f;
}
if(timer>plasmatime && weapon==Plasmathrow){
	timer=0.0f;
}
}