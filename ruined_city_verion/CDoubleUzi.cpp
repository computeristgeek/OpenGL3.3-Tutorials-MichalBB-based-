#include "mainheader.h"

CDoubleUzi gDUzi;

//====================

GLvoid CDoubleUzi::renderWeapon()
{
	glPushMatrix();
	glTranslatef(-0.35f, 0, fZ[0]);
	mWeapon.renderModel(&adWeapon);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.35f, 0, fZ[1]);
	mWeapon.renderModel(&adWeapon2);
	glPopMatrix();
	if(!glAp.key(VK_LBUTTON))
	if(fZ[iAUzi] > 0.0f)fZ[iAUzi] -= glAp.sof(2.0f);
	else fZ[iAUzi] = 0.0f;

	if(fZ[1 - iAUzi] > 0.0f)fZ[1 - iAUzi] -= glAp.sof(2.0f);
	else fZ[1 - iAUzi] = 0.0f;
}

//====================

GLvoid CDoubleUzi::renderAmmo()
{
	if(iCurWeapon != DOUBLEUZI)iAmmo = gQMinigun.iAmmo;
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	FOR(i, ESZ(bBullets))
	{
		glColor4ub(128, 128, 128, (BYTE)(bBullets[i].fLife * 255));
		glVertex3f(bBullets[i].vPos.x, bBullets[i].vPos.y, bBullets[i].vPos.z);
		CVector3 vPos2 = bBullets[i].vEnd;
		glColor3ub(220, 220, 220);
		glVertex3f(vPos2.x, vPos2.y, vPos2.z);
		CVector3 vDif1 = bBullets[i].vPos - bBullets[i].vEnd;
		bBullets[i].vPos += bBullets[i].vDir * glAp.sof(40.0f);
		CVector3 vDif2 = bBullets[i].vPos - bBullets[i].vEnd;
		bBullets[i].fLife -= glAp.sof(2.5f);
		if(vDif1.x * vDif2.x < 0.0f || vDif1.y * vDif2.y < 0.0f || vDif1.z * vDif2.z < 0.0f || bBullets[i].fLife < 0.0f)
		{
			vector<CBullet>::iterator it = bBullets.begin();
			it += i;
			bBullets.erase(it);
			i--;
		}
	}
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}

//====================

GLvoid CDoubleUzi::shoot()
{
	fZ[iAUzi] += glAp.sof(2.0f);
	if(fZ[iAUzi] > 0.22f)
	{
		mWeapon.startAnimation("fire", 1, iAUzi ? &adWeapon2 : &adWeapon);
		CVector3 vEye2 = cCam.vEye; vEye2.y += 3.0f;
		CVector3 vView2 = cCam.vView; vView2.y += 3.0f;
		CVector3 vDir = cCam.vView - cCam.vEye;
		vecNormalize(vDir);
		CVector3 vNext = cCam.vEye + vDir * 10000.0f;

		FOR(j, ESZ(gRipper.rdDiscs))
		if(collisionSphereLine(gRipper.rdDiscs[j].vPos, 2.0f, vEye2, vNext))
		{
			gRipper.rdDiscs.erase(gRipper.rdDiscs.begin() + j);
			j--;
		}

		FOR(j, ESZ(gCannon.bBalls))
		if(collisionSphereLine(gCannon.bBalls[j].vPos, 3.0f, vEye2, vNext))
			gCannon.bBalls[j].vAcc += vDir * 5.0f;

		CVector3 vTarget = getIntersectionEnemy(cCam.vEye2, cCam.vView2, getIntersection(vEye2, vView2));
		if(iLIE != -1)
		{
			if(iEnType == 0)tTowers[iLIE].fLife -= 5.0f;
			else if(iEnType == 1)
			{
				sSpiders[iLIE].fLife -= 5.0f;
				sSpiders[iLIE].addBlood(vTarget, 5.0f);
			}
		}
		pair<CVector3, CVector3> vCDirs = getNormQuad(vDir);
		GLfloat fSign = -0.5f + (float)iAUzi * 1.0f;
		CVector3 vFPos = cCam.vView2 + vDir + vCDirs.first * fSign + vCDirs.second * 0.3f;
		CVector3 vDir2 = vTarget - vFPos;
		vecNormalize(vDir2);
		CBullet bNew;
		bNew.vDir = vDir2;
		bNew.vPos = vFPos;
		bNew.vEnd = vTarget;
		bNew.fLife = 1.0f;
		bBullets.push_back(bNew);
		fZ[iAUzi] = 0.22f;
		iAUzi = 1 - iAUzi;
		iAmmo -= 1;
		FSOUND_PlaySound(FSOUND_FREE, sndData[2]);
	}
}

//====================

GLvoid CDoubleUzi::renderBlended(){}