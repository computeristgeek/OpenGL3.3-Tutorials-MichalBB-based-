#include "mainheader.h"

#define CONSTSPEED 300.0f

CRipper gRipper;

//====================

GLvoid getAngles(CVector3 vDir, GLfloat &fAnX, GLfloat &fAnY)
{
	CVector3 vDir2 = vDir; vDir2.y = 0;
	fAnY = (float)vecAngle(vDir2, CVector3(0, 0, -1)) * 180 / PI;
	if(vDir2.x < 0.0f)fAnY = 360 - fAnY;
	fAnX = (float)vecAngle(vDir, vDir2) * 180 / PI;
	if(vDir.y < 0.0f)fAnX = 360 - fAnX;
}

//====================

GLvoid CRipper::renderWeapon()
{
	if(!bLoaded && iAmmo > 0){bLoaded = true; mWeapon.startAnimation("init", 1, &adWeapon);}
	if(bLoaded && adWeapon.iAnim == -1){mWeapon.startAnimation("loop", -1, &adWeapon); bShooting = false;}
	mWeapon.renderModel(&adWeapon);
}

//====================

GLvoid CRipper::shoot()
{
	if(bShooting)return;
	if(iAmmo == 0)return;
	bShooting = true;
	bLoaded = false;
	iAmmo--;
	if(iAmmo != 0)mWeapon.startAnimation("init", 1, &adWeapon);
	else mWeapon.startAnimation("empty", -1, &adWeapon);
	FSOUND_PlaySound(FSOUND_FREE, sndData[0]);
	CRipperDisc rdNew;
	rdNew.vPos = cCam.vView + rdNew.vDir * 3; rdNew.vPos.y += 2;
	CVector3 vEye2 = cCam.vEye; vEye2.y += 3.0f;
	CVector3 vView2 = cCam.vView; vView2.y += 3.0f;
	CVector3 vTarget = getIntersection(vEye2, vView2);
	rdNew.vDir = vTarget - rdNew.vPos;
	vecNormalize(rdNew.vDir);
	rdNew.vNext = vTarget;
	rdNew.fAngle = 0.0f;
	rdNew.iFIndex = iLIF;
	rdNew.fTime = 10.0f;
	getAngles(rdNew.vDir, rdNew.fAnX, rdNew.fAnY);
	rdNew.iNumHits = 0;
	rdDiscs.push_back(rdNew);
}

//====================

GLvoid CRipper::renderAmmo()
{
	if(iCurWeapon != RIPPER)bLoaded = false;
	FOR(i, ESZ(rdDiscs))
	{
		rdDiscs[i].vLastPos = rdDiscs[i].vPos;
		glPushMatrix();
		glTranslatef(rdDiscs[i].vPos.x, rdDiscs[i].vPos.y, rdDiscs[i].vPos.z);
		glRotatef(-rdDiscs[i].fAnY, 0, 1, 0);
		glRotatef(rdDiscs[i].fAnX, 1, 0, 0);
		rdDiscs[i].fAngle += glAp.sof(720.0f); while(rdDiscs[i].fAngle >= 360.0f)rdDiscs[i].fAngle -= 360.0f;
		mRDisc.renderModel(NULL);
		glPopMatrix();
		CVector3 vDifA = rdDiscs[i].vNext - rdDiscs[i].vPos;
		rdDiscs[i].vPos += rdDiscs[i].vDir * CONSTSPEED * glAp.sof(1.0f);
		CVector3 vDifB = rdDiscs[i].vNext - rdDiscs[i].vPos;
		if(rdDiscs[i].iFIndex != -1)
		if(vDifA.x * vDifB.x < 0.0f || vDifA.y * vDifB.y < 0.0f || vDifA.z * vDifB.z < 0.0f)
		{
			FSOUND_PlaySound(FSOUND_FREE, sndData[1]);
			GLfloat fBehind = vecDist(rdDiscs[i].vNext, rdDiscs[i].vPos) + 0.1f;
			rdDiscs[i].vDir = lv1.vNorms[rdDiscs[i].iFIndex] * 2.0f * vecDot(rdDiscs[i].vDir * -1.0f, lv1.vNorms[rdDiscs[i].iFIndex]) + rdDiscs[i].vDir;
			rdDiscs[i].vPos = rdDiscs[i].vNext + rdDiscs[i].vDir * fBehind;
			rdDiscs[i].vNext = getIntersection(rdDiscs[i].vPos, rdDiscs[i].vPos + rdDiscs[i].vDir);
			rdDiscs[i].iFIndex = iLIF;
			rdDiscs[i].iNumHits++;
			getAngles(rdDiscs[i].vDir, rdDiscs[i].fAnX, rdDiscs[i].fAnY);
		}
		rdDiscs[i].fTime -= glAp.sof(1.0f);
		if(rdDiscs[i].iNumHits == 5 || rdDiscs[i].fTime < 0.0f)
		{
			rdDiscs.erase(rdDiscs.begin() + i);
			i--;
			continue;
		}
		GLint iCollide = collisionLineEnemies(rdDiscs[i].vPos, rdDiscs[i].vLastPos);
		if(iCollide)
		{
			rdDiscs.erase(rdDiscs.begin() + i);
			takeEnemiesLife(34.0f);
		}
	}
}

//====================

GLvoid CRipper::renderBlended(){}