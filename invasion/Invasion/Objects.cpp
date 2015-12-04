#include "Main.h"

CItem health,missilepack,bazooka2; // Create our objects
float mt=0.3f; // Time for shooting missiles

void CreateRandomObject(const Enemy &enemy)
{
	int object=rand()%25; // Random number

	if(object==0)health.push_back(CVector3(enemy.pos)); // Create health
	else if(object==1)missilepack.push_back(CVector3(enemy.pos));// Create missiles
	else if(object==2 && mt==0.3f && bazooka2.size()==0)bazooka2.push_back(CVector3(enemy.pos));// Create upgrading bazooka bonus
}


bool ItemCameraCollision(CCamera &camera, CVector3 &vVector, float distance)
{
	if(camera.m_vPosition.x+distance>vVector.x && camera.m_vPosition.x-distance<vVector.x &&
	   camera.m_vPosition.y+distance>vVector.y && camera.m_vPosition.y-distance<vVector.y &&
	   camera.m_vPosition.z+distance>vVector.z && camera.m_vPosition.z-distance<vVector.z)
	   return true;
	return false;
}


void DeleteItem(CItem &item, int num)
{
	CVector3 temp=item[num];
	item[num]=item[item.size()-1];
	item[item.size()-1]=temp;
	item.pop_back();
}
	


void DrawAllObjects()
{
	static float angle;
	for(int h=0;h<(int)health.size();h++) // Draw health
	{
		if(health[h].y>2)health[h].y-=0.5f;
		glPushMatrix();
		glTranslatef(health[h].x,health[h].y,health[h].z);
		glRotatef(angle,0,1,0);
		pModel[3]->draw();
		glPopMatrix();
		if(ItemCameraCollision(Main,health[h],3))
		{
			life+=10;
			DeleteItem(health,h);
			PlaySound("Data/Sounds/Took.wav",NULL,SND_ASYNC);
		}	
	}

	for(int mp=0;mp<missilepack.size();mp++) // Draw missile packs
	{
		if(missilepack[mp].y>2)missilepack[mp].y-=0.5f;
		glPushMatrix();
		glTranslatef(missilepack[mp].x,missilepack[mp].y,missilepack[mp].z);
		glRotatef(angle,0,1,0);
		pModel[2]->draw();
		glEnd();
		glPopMatrix();

		if(ItemCameraCollision(Main,missilepack[mp],3))
		{
			missile+=5;
			DeleteItem(missilepack,mp);
			PlaySound("Data/Sounds/Took.wav",NULL,SND_ASYNC);
		}
	}

	for(int b2=0;b2<bazooka2.size();b2++) // Draw bazooka upgrade
	{
		if(bazooka2[b2].y>2)bazooka2[b2].y-=0.5f;
		glPushMatrix();
		glTranslatef(bazooka2[b2].x,bazooka2[b2].y,bazooka2[b2].z);
		glRotatef(angle,0,1,0);
		glBegin(GL_QUADS);
		glColor3f(1,0,1);
		glVertex2f(-3,3);
		glVertex2f(3,3);
		glVertex2f(3,-3);
		glVertex2f(-3,-3);
		glColor3f(1,1,1);
		glEnd();
		glPopMatrix();

		if(ItemCameraCollision(Main,bazooka2[b2],3))
		{
			mt=0.15f;
			DeleteItem(bazooka2,b2);
			PlaySound("Data/Sounds/Took.wav",NULL,SND_ASYNC);
		}
	}

	angle+=2;
}
		