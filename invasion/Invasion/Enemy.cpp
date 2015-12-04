//************************************** Enemy.cpp ********************************************
// 
//			Here are function declarations that works with enemy data type.
//
//************************************** Enemy.cpp ********************************************

#include "Main.h"



////////////////////////////////////// ENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Here are our class Enemy constructors.
//
////////////////////////////////////// ENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Enemy::Enemy()
{
	pos=inc=CVector3(); // Set position and inc to 0,0,0
	life=0;		// Set life
	lastshoottime=0;
	Draw=NULL;
}

Enemy::Enemy(CVector3 POS, int LIFE, void(*draw)())
{
	pos=POS;
	life=LIFE;
	Draw=draw;
	lastshoottime=0;
}

/////////////////////////////////// ENEMY:: DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Main drawing function of enemy class.
//
/////////////////////////////////// ENEMY:: DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Enemy::DrawMe()
{
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	Draw();
	glPopMatrix();

	inc=Main.m_vPosition-pos;
	Normalize(inc);
	inc=inc/2.5f;

	pos+=inc;

	if(Distance(pos,Main.m_vPosition)<80)Shoot(400,Mlasers,DrawRedLaser);

}

//////////////////////////////////// ENEMY:: SHOOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								Enemy shooting function.
//
//////////////////////////////////// ENEMY:: SHOOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Enemy::Shoot(float millisec, CAmmo &ammo,void(*DRAW)())
{	
	if(lastshoottime+millisec<GetTickCount())
	{
		lastshoottime=GetTickCount();
		ammo.push_back(Ammo(pos,inc*2.5f,2,DRAW,Main.anX,Main.anY));
	}
	

}

////////////////////////////////////// DELETEENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function deletes Enemy class type from CEnemy.
//
////////////////////////////////////// DELETEENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DeleteEnemy(CEnemy &enemy, int num)
{
	Enemy temp=enemy[num];
	enemy[num]=enemy[enemy.size()-1]; // Get the last element
	enemy[enemy.size()-1]=temp;
	enemy.pop_back();
}

/////////////////////////////////////// DELETEAMMO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function deletes Ammo class type from CAmmo.
//
/////////////////////////////////////// DELETEAMMO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DeleteAmmo(CAmmo &ammo, int num)
{
	Ammo temp=ammo[num];
	ammo[num]=ammo[ammo.size()-1]; // Get the last element
	ammo[ammo.size()-1]=temp;
	ammo.pop_back();
}

////////////////////////////////////// TESTCOLLISION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This test collision between ammo and enemy.
//
////////////////////////////////////// TESTCOLLISION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void TestCollision(Enemy &enem,CAmmo &ammo,float xp,float yp,float zp)
{
	for(int a=0;a<(int)ammo.size();a++)
	{
		if(ammo[a].pos.x+xp>enem.pos.x && ammo[a].pos.x-xp<enem.pos.x &&
			ammo[a].pos.y+yp>enem.pos.y && ammo[a].pos.y-yp<enem.pos.y &&
			ammo[a].pos.z+zp>enem.pos.z && ammo[a].pos.z-zp<enem.pos.z)
		{enem.life-=ammo[a].damage; DeleteAmmo(ammo,a);}
	}
}

///////////////////////////////////////// DRAWENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function draws any CEnemy unit type.
//
///////////////////////////////////////// DRAWENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CExplo mart; // Extern explosion object

void DrawEnemy(CEnemy &enemy)
{
	for(int e=0;e<(int)enemy.size();e++)
	{
		enemy[e].DrawMe();
		TestCollision(enemy[e],Glasers,2,2,2);
		TestCollision(enemy[e],missiles,2,2,2);
		if(enemy[e].life<0){mart.push_back(Explosion(enemy[e].pos));CreateRandomObject(enemy[e]);DeleteEnemy(enemy,e);allkills+=1; PlaySound("Data/Sounds/Explode.wav",NULL,SND_ASYNC);}

	}
	
}

////////////////////////////////// EXPLOSION::SIMULATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						    	This function draws explosion.
//
////////////////////////////////// EXPLOSION::SIMULATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Explosion::Simulate()
{
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glRotatef(Main.anY,0,1,0);
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	glBegin(GL_QUADS);
	glColor3f(1,0.3f,0);
	glTexCoord2f(0,1); glVertex2f(-size,size);
	glTexCoord2f(1,1); glVertex2f(size,size);
	glTexCoord2f(1,0); glVertex2f(size,-size);
	glTexCoord2f(0,0); glVertex2f(-size,-size);
	glEnd();
	glColor3f(1,1,1);
	glPopMatrix();
	size+=0.1f;
}

////////////////////////////////// DELETEEXPLOSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						    	This function deletes explosion.
//
////////////////////////////////// DELETEEXPLOSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DeleteExplosion(CExplo &explo, int num)
{
	Explosion temp=explo[num];
	explo[num]=explo[explo.size()-1];
	explo[explo.size()-1]=temp;
	explo.pop_back();
}

/////////////////////////////////// DRAWEXPLOSIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						  	This function draws explosions.
//
/////////////////////////////////// DRAWEXPLOSIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawExplosions(CExplo &explo)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	for(int e=0;e<(int)explo.size();e++)
	{
		explo[e].Simulate();
		if(explo[e].size>20)DeleteExplosion(explo,e);
	}
	glDisable(GL_BLEND);
}
