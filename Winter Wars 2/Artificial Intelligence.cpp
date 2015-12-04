#include "main.h"
#include "Weapons.h"
#include "Camera.h"

Player Enemy;


Ammo enemolas[100];




void EnemyAmmo(){
	for(int las=0;las<enemyammo;las++){
glPushMatrix();
if(!enemolas[las].active){
	enemolas[las].active=TRUE;
	enemolas[las].x=Enemy.x;
	enemolas[las].z=Enemy.z;
	enemolas[las].y=2;
	enemolas[las].xinc=Enemy.x-camerax;
	enemolas[las].zinc=Enemy.z-cameraz;
}
glTranslatef(enemolas[las].x,enemolas[las].y,enemolas[las].z);
glBindTexture(GL_TEXTURE_2D, g_Texture[6]);
glCallList(a[2]);
glPopMatrix();
enemolas[las].x-=enemolas[las].xinc/15;
enemolas[las].z-=enemolas[las].zinc/15;
}

}

float time;
void EnemyTime(){

	if(time==0.1f){enemyammo+=1;}
	if(time>0.55f){time=0;}
	time+=0.1f;
}
int action=2,obj;
bool once=true;
float timea=0;






void EnemyIQ(){

switch(action){
case 0:
	action=3;
case 1:
	once=true;
	if(SuperHealth.active){
	Enemy.z-=(Enemy.z-SuperHealth.z)/50;
	Enemy.x-=(Enemy.x-SuperHealth.x)/50;
	}else{action=2;}
break;
case 2:
	once=true;



break;
case 3:
	if(once){
		once=FALSE;
obj=rand()%9;
	}
	if(Armor[obj].active){
Enemy.z-=(Enemy.z-Armor[obj].z)/50;
Enemy.x-=(Enemy.x-Armor[obj].x)/50;
	}
	else obj=rand()%9;
break;

	}

timea+=0.01f;
if(timea>3){
	timea=0;
	action=rand()%4;
}
}

void DoWhatYouReSupposedToDo(){

EnemyIQ();
if(Enemy.armor<0){Enemy.armor=0; Enemy.life+=Enemy.armor;}






if(Enemy.life<=0){
myfrag+=1;	Enemy.life=100;Enemy.x=-100+(rand()%100)*1.0f;Enemy.z=-100+(rand()%300)*1.0f;}
for(int l=0;l<100;l++){
if(enemolas[l].z+1>cameraz && enemolas[l].z-1<cameraz && enemolas[l].x+1>camerax && enemolas[l].x-1<camerax && enemolas[l].active && armora==0)
{enemolas[l].active=FALSE; enemyammo-=1; life-=4;}
else if(enemolas[l].z+1>cameraz && enemolas[l].z-1<cameraz && enemolas[l].x+1>camerax && enemolas[l].x-1<camerax && enemolas[l].active && armora>0){
	enemolas[l].active=FALSE; enemyammo-=1; armora-=4;}
if((enemolas[l].x<-100 || enemolas[l].x>100 || enemolas[l].z<-100 || enemolas[l].z>100) && enemolas[l].active){enemolas[l].active=FALSE; enemyammo-=1;}
if(armora<0){life+=armora;armora=0;}



for(int arm=0;arm<ARMOR;arm++){
if(Enemy.x-1<Armor[arm].x && Enemy.x+1>Armor[arm].x && Enemy.z-1<Armor[arm].z && Enemy.z+1>Armor[arm].z && Armor[arm].active)
{Armor[arm].active=false;Enemy.armor+=25;}

	}
}
if(Enemy.x-1<SuperHealth.x && Enemy.x+1>SuperHealth.x && Enemy.z-1<SuperHealth.z && Enemy.z+1>SuperHealth.z && SuperHealth.active)
{SuperHealth.active=false;Enemy.life+=100;}

if(life<=0)	{g_Camera.PositionCamera(0, 1.75f, 6,   0, 1.75f, 0,   0, 1, 0);;RocketLauncher.taken=FALSE;PlasmaThrower.taken=FALSE; weapon=Lasergun; compfrag+=1; life=100; ammol=70; ammorocket=5;ammoplasma=10;}
}


void EnemyMain(){

EnemyTime();
EnemyAmmo();
glPushMatrix();
glTranslatef(Enemy.x,Enemy.y,Enemy.z);
glScalef(0.03f,0.03f,0.03f);
if(Enemy.life>0){

DrawEnemy();
}

glPopMatrix();
}