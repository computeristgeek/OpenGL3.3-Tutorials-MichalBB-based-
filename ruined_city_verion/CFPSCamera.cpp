#include "mainheader.h"

//====================

void axisRotation(float fAngle, CVector3 vAxis, CVector3 &vPoint, CVector3 vCenter)
{
	CVector3 vNewView;
	CVector3 vView = vPoint - vCenter;

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(fAngle);
	float sinTheta = (float)sin(fAngle);

	float x = vAxis.x, y = vAxis.y, z = vAxis.z;

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	vPoint = vCenter + vNewView;
}

//====================

CFPSCamera::CFPSCamera(CVector3 svEye, CVector3 svView, CVector3 svUp, float sfHeight, float sfSense, float sfSpeed)
{
	vEye = svEye;
	vView = svView;
	vUp = svUp;
	fHeight = sfHeight;
	fSense = sfSense;
	fSpeed = sfSpeed;
	fBobAn = vAcc.y = 0.0f;
	bStanding = false;
}

//====================

void CFPSCamera::moveByMouse()
{
	GetCursorPos(&pMPCur);
	RECT rRect; GetWindowRect(glAp.hWindow, &rRect);
	int iCentX = rRect.left + ((rRect.right - rRect.left) >> 1),
		iCentY = rRect.top + ((rRect.bottom - rRect.top) >> 1);

	float deltaX = (float)(iCentX - pMPCur.x) * fSense / 20.0f;
	float deltaY = (float)(iCentY - pMPCur.y) * fSense / 20.0f;

	if(deltaX != 0.0f)axisRotation(deltaX * (PI / 180), CVector3(0, 1, 0), vView, vEye);
	if(deltaY != 0.0f)
	{
		CVector3 vAxis = vecCross(vView - vEye, vUp);
		vecNormalize(vAxis);
		float fAngle = deltaY;
		float fNewAngle = fAngle + getAngleX();
		if(fNewAngle > -89.80f && fNewAngle < 89.80f)
		{
			fAngle *= PI / 180;
			axisRotation(fAngle, vAxis, vView, vEye);
		}
	}
	SetCursorPos(iCentX, iCentY);
}

//====================

float CFPSCamera::getAngleY()
{
	CVector3 vDir = vView - vEye; vDir.y = 0.0f;
	vecNormalize(vDir);
	float fAngle = (float)vecAngle(CVector3(0, 0, -1), vDir) * (180 / PI);
	if(vDir.x < 0)fAngle = 360.0f - fAngle;
	return fAngle;
}

//====================

float CFPSCamera::getAngleX()
{
	CVector3 vDir = vView - vEye;
	vecNormalize(vDir);
	CVector3 vDir2 = vDir; vDir2.y = 0.0f;
	vecNormalize(vDir2);
	float fAngle = (float)vecAngle(vDir, vDir2) * (180 / PI);
	if(vDir.y < 0)fAngle *= -1.0f;
	return fAngle;
}

//====================

void CFPSCamera::setMoving(int siForw, int siBack, int siLeft, int siRight)
{
	iForw = siForw;
	iBack = siBack;
	iLeft = siLeft;
	iRight = siRight;
}

//====================

void sldown(float &fValue, float fSpeed)
{
	if(fValue != 0.0f)
	{
		float fSign1 = fValue > 0.0f ? 1.0f : -1.0f;
		fValue -= glAp.sof(fSpeed) * fSign1;
		float fSign2 = fValue > 0.0f ? 1.0f : -1.0f;
		if(fSign1 * fSign2 < 0.0f)fValue = 0.0f;
	}
}

void CFPSCamera::update()
{
	moveByMouse();
	CVector3 vMove = vView - vEye; vMove.y = 0.0f;
	vecNormalize(vMove); vMove *= fSpeed;

	CVector3 vStrafe = vecCross(vView - vEye, vUp);
	vecNormalize(vStrafe); vStrafe *= fSpeed;

	int iMove = 0;
	CVector3 vMoveBy;
	if(glAp.key(iForw)){vMoveBy += vMove * glAp.sof(1.0f); iMove |= 1;}
	if(glAp.key(iBack)){vMoveBy -= vMove * glAp.sof(1.0f); iMove |= 1;}
	if(glAp.key(iLeft)){vMoveBy -= vStrafe * glAp.sof(1.0f); iMove |= 1;}
	if(glAp.key(iRight)){vMoveBy += vStrafe * glAp.sof(1.0f);  iMove |= 1;}
	if(glAp.key(' ') && vAcc.y == 0.0f)vAcc.y = 30.0f;

	vMoveBy += vAcc * glAp.sof(1.0f);
	sldown(vAcc.x, 100.0f);
	vAcc.y -= glAp.sof(70.0f);
	sldown(vAcc.z, 100.0f);
	checkCollision(vMoveBy);

	if(iMove)fBobAn += glAp.sof(3*PI);
	vEye2 = vEye; vEye2.y += 3.0f;
	vView2 = vView; vView2.y += 3.0f;
	pair<CVector3, CVector3> vQuad = getNormQuad(vView2 - vEye2);
	vQuadA = vQuad.first;
	vQuadB = vQuad.second;
}

//====================

void CFPSCamera::resetMouse()
{
	RECT rRect; GetWindowRect(glAp.hWindow, &rRect);
	int iCentX = rRect.left + ((rRect.right - rRect.left) >> 1),
		iCentY = rRect.top + ((rRect.bottom - rRect.top) >> 1);
	SetCursorPos(iCentX, iCentY);
}

//====================

void CFPSCamera::look()
{
	gluLookAt(vEye.x, vEye.y + 3, vEye.z,
				vView.x, vView.y + 3, vView.z,
				vUp.x, vUp.y, vUp.z);
}

void CFPSCamera::checkCollision(CVector3 vMoveBy)
{
	CVector3 vCenter = vEye;
	vCenter.y += vMoveBy.y;
	vEye += vMoveBy;
	vView += vMoveBy;
	
	FOR(j, ESZ(lv1.iStand))
	{
		int i = lv1.iStand[j];
		CVector3 vPoly[] = {lv1.vFaces[i*3], lv1.vFaces[i*3 + 1], lv1.vFaces[i*3 + 2]};
		float fDFCenter = 0.0f;
		if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[i], vEye, 3, fDFCenter))
		{
			CVector3 vOffset = lv1.vNorms[i];
			if(fDFCenter > 0)vOffset *= (3 - fDFCenter);
			else vOffset *= (3 + fDFCenter + 0.1f) * -1;
			vEye += vOffset;
			vView += vOffset;
			vAcc.y = 0.0f;
		}
	}

	FOR(j, ESZ(lv1.iNotStand))
	{
		int i = lv1.iNotStand[j];
		CVector3 vPoly[] = {lv1.vFaces[i*3], lv1.vFaces[i*3 + 1], lv1.vFaces[i*3 + 2]};
		float fDFCenter = 0.0f;
		if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[i], vEye, 3, fDFCenter))
		{
			CVector3 vOffset = lv1.vNorms[i];
			if(fDFCenter > 0)vOffset *= (3 - fDFCenter);
			else vOffset *= (3 + fDFCenter) * -1;
			vEye += vOffset;
			vView += vOffset;
		}
	}
}