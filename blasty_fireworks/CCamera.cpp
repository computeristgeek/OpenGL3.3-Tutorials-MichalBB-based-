#include "mainheader.h"

//====================

GLvoid axisRotation(GLfloat fAngle, CVector3 vAxis, CVector3 &vPoint, CVector3 vCenter)
{
	CVector3 vNewView;
	CVector3 vView = vPoint - vCenter;

	// Calculate the sine and cosine of the angle once
	GLfloat cosTheta = (float)cos(fAngle);
	GLfloat sinTheta = (float)sin(fAngle);

	GLfloat x = vAxis.x, y = vAxis.y, z = vAxis.z;

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

CCamera::CCamera(CVector3 svEye, CVector3 svView, CVector3 svUp, GLfloat sfSense, GLfloat sfSpeed)
{
	vEye = svEye;
	vView = svView;
	vUp = svUp;
	fSense = sfSense;
	fSpeed = sfSpeed;
}

//====================

GLvoid CCamera::rotateByMouse()
{
	GetCursorPos(&pMPCur);
	RECT rRect; GetWindowRect(glAp.hWindow, &rRect);
	GLint iCentX = rRect.left + ((rRect.right - rRect.left) >> 1),
		iCentY = rRect.top + ((rRect.bottom - rRect.top) >> 1);

	GLfloat deltaX = (float)(iCentX - pMPCur.x)*(fSense/100.0f);
	GLfloat deltaY = (float)(iCentY - pMPCur.y)*(fSense/100.0f);

	if(deltaX != 0.0f)axisRotation(deltaX * (PI / 180), CVector3(0, 1, 0), vView, vEye);
	if(deltaY != 0.0f)
	{
		CVector3 vAxis = vecCross(vView - vEye, vUp);
		vecNormalize(vAxis);
		GLfloat fAngle = deltaY;
		GLfloat fNewAngle = fAngle + getAngleX();
		if(fNewAngle > -89.80f && fNewAngle < 89.80f)
		{
			fAngle *= PI / 180;
			axisRotation(fAngle, vAxis, vView, vEye);
		}
	}
	SetCursorPos(iCentX, iCentY);
}

//====================

GLfloat CCamera::getAngleY()
{
	CVector3 vDir = vView - vEye; vDir.y = 0.0f;
	vecNormalize(vDir);
	GLfloat fAngle = (float)vecAngle(CVector3(0, 0, -1), vDir) * (180 / PI);
	if(vDir.x < 0)fAngle = 360.0f - fAngle;
	return fAngle;
}

//====================

GLfloat CCamera::getAngleX()
{
	CVector3 vDir = vView - vEye;
	vecNormalize(vDir);
	CVector3 vDir2 = vDir; vDir2.y = 0.0f;
	vecNormalize(vDir2);
	GLfloat fAngle = (float)vecAngle(vDir, vDir2) * (180 / PI);
	if(vDir.y < 0)fAngle *= -1.0f;
	return fAngle;
}

//====================

GLvoid CCamera::setMoving(GLint siForw, GLint siBack, GLint siLeft, GLint siRight)
{
	iForw = siForw;
	iBack = siBack;
	iLeft = siLeft;
	iRight = siRight;
}

//====================

GLvoid CCamera::update()
{
	rotateByMouse();
	CVector3 vMove = vView - vEye; vMove.y = 0.0f;
	vecNormalize(vMove); vMove *= fSpeed;

	CVector3 vStrafe = vecCross(vView - vEye, vUp);
	vecNormalize(vStrafe); vStrafe *= fSpeed;

	GLint iMove = 0;
	CVector3 vMoveBy = CVector3();
	if(glAp.key(iForw))vMoveBy += vMove*glAp.sof(1.0f);
	if(glAp.key(iBack))vMoveBy -= vMove*glAp.sof(1.0f);
	if(glAp.key(iLeft))vMoveBy -= vStrafe*glAp.sof(1.0f);
	if(glAp.key(iRight))vMoveBy += vStrafe*glAp.sof(1.0f);
	vEye += vMoveBy; vView += vMoveBy;
}

//====================

GLvoid CCamera::resetMouse()
{
	RECT rRect; GetWindowRect(glAp.hWindow, &rRect);
	GLint iCentX = rRect.left + ((rRect.right - rRect.left) >> 1),
		iCentY = rRect.top + ((rRect.bottom - rRect.top) >> 1);
	SetCursorPos(iCentX, iCentY);
}

//====================

GLvoid CCamera::look()
{
	gluLookAt(vEye.x, vEye.y, vEye.z,
				vView.x, vView.y, vView.z,
				vUp.x, vUp.y, vUp.z);
}

//====================

GLvoid CCamera::checkCollision()
{
	for(GLint i = 0; i < ESZ(wWorld.vVerts); i += 3)
	{
		CVector3 vPoly[] = {wWorld.vVerts[i], wWorld.vVerts[i+1], wWorld.vVerts[i+2]};
		GLfloat fDFCenter = 0.0f;
		if(collisionSpherePoly(vPoly, 3, &wWorld.vNorms[i/3], vEye, 3, fDFCenter))
		{
			CVector3 vOffset = wWorld.vNorms[i/3];
			if(fDFCenter > 0)vOffset *= (3 - fDFCenter);
			else vOffset *= (3 + fDFCenter) * -1;
			vEye += vOffset;
			vView += vOffset;
		}
	}
	FOR(i, ESZ(wWorld.oObjects))
	{
		for(GLint j = 0; j < ESZ(wWorld.oObjects[i].vBoundBox); j += 4)
		{
			CVector3 vPoly[] = {wWorld.oObjects[i].vBoundBox[j], wWorld.oObjects[i].vBoundBox[j+1], 
				wWorld.oObjects[i].vBoundBox[j+2], wWorld.oObjects[i].vBoundBox[j+3]};
			GLfloat fDFCenter = 0.0f;
			if(collisionSpherePoly(vPoly, 4, &wWorld.oObjects[i].vNorms[j/4], vEye, 3, fDFCenter))
			{
				CVector3 vOffset = wWorld.oObjects[i].vNorms[j/4];
				if(fDFCenter > 0)vOffset *= (3 - fDFCenter);
				else vOffset *= (3 + fDFCenter) * -1;
				vEye += vOffset;
				vView += vOffset;
			}
		}
	}
}