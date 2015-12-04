//************************************** Ammo.cpp *********************************************
// 
//			Here are function declarations that works with ammo data type.
//
//************************************** Ammo.cpp *********************************************

#include "Main.h"

////////////////////////////////////// AMMO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Here are our class Ammo constructors.
//
////////////////////////////////////// AMMO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\* 

Ammo::Ammo() // Here we define our default ammo constructor
{
	damage=rotx=roty=0; // Set damage and rotations to 0
	Draw=NULL; // Set function pointer to NULL
	pos=inc=CVector3(); // Position and increasing are also ste to 0
	follow=-1;
};

Ammo::Ammo(CVector3 POS, CVector3 INC, int dam, void(*draw)(), float rotX,float rotY)
{
	pos=POS; // Set position
	inc=INC; // Set increase data
	damage=dam; // Set ammo's damage
	Draw=draw;  // Set drawing function 
	rotx=rotX; // Set angles
	roty=rotY;
	follow=-1;
}

///////////////////////////////// AMMO:: DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Main drawing function of ammo class.
//
///////////////////////////////// AMMO:: DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Ammo::DrawMe()
{
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z); // Move object
	glRotatef(roty,0,1,0); // Rotate object on Y-axis
	glRotatef(rotx,1,0,0); // Rotate object on X-axis
	Draw(); // Draw it
	if(follow!=-1)
	{
		inc=martian[follow].pos-pos;
		Normalize(inc);
	}
	pos+=inc; // Update its position
	glPopMatrix();
}

///////////////////////////////////////// DRAWAMMO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function draws any CAmmo unit type.
//
///////////////////////////////////////// DRAWAMMO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawAmmo(CAmmo &ammo)
{
	for(int a=0;a<(int)ammo.size();a++)
	{
		ammo[a].DrawMe();
		if(AmmoOutOfWorld(ammo[a]))DeleteAmmo(ammo,a);
	}
}

//////////////////////////////////// PARTICLE::DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function draws any particle.
//
//////////////////////////////////// PARTICLE::DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Particle::DrawMe()
{
	glBegin(GL_QUADS);
	glColor3f(1,0.5f,0);
	glVertex3f(pos.x-0.1f,pos.y+0.1f,pos.z);
	glVertex3f(pos.x+0.1f,pos.y+0.1f,pos.z);
	glVertex3f(pos.x+0.1f,pos.y-0.1f,pos.z);
	glVertex3f(pos.x-0.1f,pos.y-0.1f,pos.z);
	glColor3f(1,1,1);
	glEnd();

	pos+=inc;
	life-=0.01f;
}

//////////////////////////////////// DELETEPARTICLE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function deletes any particle.
//
//////////////////////////////////// DELETEPARTICLE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DeleteParticle(CPart &part,int num)
{
	Particle temp=part[num];
	part[num]=part[part.size()-1];
	part[part.size()-1]=temp;
	part.pop_back();
}

///////////////////////////////////// DRAWPARTICLES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function draws any CPart unit type.
//
///////////////////////////////////// DRAWPARTICLES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawParticles(CPart &part)
{
	glPushMatrix();
	for(int p=0;p<(int)part.size();p++)
	{
		part[p].DrawMe();
		if(part[p].life<0)DeleteParticle(part,p);
	}
	glPopMatrix();

}

////////////////////////////////// MISSILEBASE::DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									Draws missile base.
//
////////////////////////////////// MISSILEBASE::DRAWME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void MissileBase::DrawMe()
{
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	pModel[4]->draw();
	glPopMatrix();
}

////////////////////////////////// MISSILEBASE::SHOOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							This shoots ammo from missile base.
//
////////////////////////////////// MISSILEBASE::SHOOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void MissileBase::Shoot(CAmmo &ammo,float delay)
{
	static float last=GetTickCount();
	float current=GetTickCount();
	if(last+delay<current)
	{
		last=GetTickCount();
		ammo.push_back(Ammo(pos,CVector3(),100,DrawMissile,0,Main.anY));
		count+=1;
	}
}
 
///////////////////////////////////////// DRAWMB \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								This draws any CMb unit type.
//
///////////////////////////////////////// DRAWMB \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawMB(CMb &mb)
{
	for(int m=0;m<(int)mb.size();m++)
	{
		mb[m].DrawMe();
		mb[m].Shoot(missiles,100);
		if(mb[m].count>150){mart.push_back(Explosion(mb[m].pos));mb.pop_back();}
	}
}
