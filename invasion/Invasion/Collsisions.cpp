#include "Main.h"

//////////////////////////////////////// DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					This function returns distance between two points.
//
//////////////////////////////////////// DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Distance(CVector3 vPoint1, CVector3 vPoint2)
{
	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						    (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );

	// Return the distance between the 2 points
	return (float)distance;
}

////////////////////////////////////////// GRAND \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							        Great random function.
//
////////////////////////////////////////// GRAND \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float grand(int num)
{
	return rand()%2 ? (float)(rand()%num) : (float)-(rand()%num);
}

//////////////////////////////////// AMMOOUTOFWORLD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Returns true if ammo is out of the world.
//
//////////////////////////////////// AMMOOUTOFWORLD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool AmmoOutOfWorld(Ammo &ammo)
{
	if(ammo.pos.x>400 || ammo.pos.x<-400 ||
	   ammo.pos.y<0 || ammo.pos.y>200    ||
	   ammo.pos.z>400 || ammo.pos.z<-400)return true;
	else return false;
}

//////////////////////////////////// CAMERAINWORLD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Returns true if camera is in the world.
//
//////////////////////////////////// CAMERAINWORLD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CameraInWorld(CCamera &camera)
{
	if(camera.m_vPosition.x>-399 && camera.m_vPosition.x<399 &&
	   camera.m_vPosition.z<399  && camera.m_vPosition.z>-399)
	   return true;

	else {camera.PositionCamera(0,3,0,	0,3,-0.5f,	0,1,0);camera.anX=camera.anY=0;}
	return false;
	
}

//////////////////////////////////// CAMERAINWORLD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This checks collision between ammo and camera.
//
//////////////////////////////////// CAMERAINWORLD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CheckAmmoAndCamera(CCamera &camera, CAmmo &ammo)
{
	for(int a=0;a<(int)ammo.size();a++)
	{
		if(camera.m_vPosition.x+2>ammo[a].pos.x && camera.m_vPosition.x-2<ammo[a].pos.x &&
		   camera.m_vPosition.y+2>ammo[a].pos.y && camera.m_vPosition.y-1<ammo[a].pos.y &&
		   camera.m_vPosition.z+2>ammo[a].pos.z && camera.m_vPosition.z-2<ammo[a].pos.z)
		{
			DeleteAmmo(ammo,a); life-=2;
		}
	}
}
	