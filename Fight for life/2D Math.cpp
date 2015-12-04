//************************************* 2D math.cpp *******************************************
// 
//								Here are 2D math functions.
//
//************************************* 2D math.cpp *******************************************

#include "Main.h"

const float PI=3.1428f; // Create PI variable

/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								Returns the magnitude of a vector.
//
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(CVector2 vVector)
{
	return (float)sqrt( ( vVector.x*vVector.x)+
						( vVector.y*vVector.y));
}

/////////////////////////////////////////// DOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								Returns the dot of two vectors.
//
/////////////////////////////////////////// DOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Dot(CVector2 vVector1, CVector2 vVector2) 
{
	return ((vVector1.x * vVector2.x) + (vVector1.y * vVector2.y));
}

///////////////////////////////////// ROTATEAFTER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Returns angle needed to rotate after some point.
//
///////////////////////////////////// ROTATEAFTER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float RotateAfter(CVector2 from, CVector2 after)
{
	bool add180=false;
	if(after.x<from.x)add180=true;
	CVector2 vDirect=after-from;
	vDirect/=Magnitude(vDirect);
	float angle=(float)acos(Dot(CVector2(0,1),vDirect));
	if(_isnan(angle))return 0;
	angle*=(180/PI);
	if(add180)angle=360-angle;
	return angle;
}

//////////////////////////////////////// DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							Returns distance between two points.
//
//////////////////////////////////////// DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Distance(CVector2 &vPoint1, CVector2 &vPoint2)
{
	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y));
	// Return the distance between the 2 points
	return (float)distance;
}

/////////////////////////////////////// CBCOLLIDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Returns true is two collision boxes collided.
//
/////////////////////////////////////// CBCOLLIDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CBCollide(CVector2 &b1, CVector2 &b2,CVector2 &pos1, CVector2 &pos2)
{
	if( pos1.x+b1.x>pos2.x-b2.x && pos1.x-b1.x<pos2.x+b2.x && 
		pos1.y+b1.y>pos2.y-b2.y && pos1.y-b1.y<pos2.y+b2.y)
		return true;
	return false;
}

// v- returns a speed optimized vector

CVector2 v(CVector2 vInc)
{
	return CVector2(f(vInc.x),f(vInc.y));
}
