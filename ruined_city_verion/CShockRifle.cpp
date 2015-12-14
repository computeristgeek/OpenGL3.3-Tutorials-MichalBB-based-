#include "mainheader.h"

GLuint uiExpTex, uiPartTex, uiSmokeTex;
CShockRifle gShockRifle;

//====================

GLvoid CShockRifle::renderWeapon()
{
	mWeapon.renderModel(&adWeapon);
}

//====================

GLvoid CShockRifle::renderAmmo()
{
}

//====================

GLvoid CShockRifle::shoot()
{
	if(fTime == 0.0f && iAmmo > 0)
	{
		CVector3 vEye2 = cCam.vEye, vView2 = cCam.vView; vEye2.y += 3.0f; vView2.y += 3.0f;
		CVector3 vBest = getIntersectionEnemy(vEye2, vView2, getIntersection(vEye2, vView2));
		if(iLIE != -1)
		{
			if(iEnType == 0)tTowers[iLIE].fLife -= 80.0f;
			else if(iEnType == 1)
			{
				sSpiders[iLIE].fLife -= 80.0f;
				sSpiders[iLIE].addBlood(vBest, 75.0f);
			}
		}
		CImpact iNew;
		iNew.cOnlyBeam = iLIF == -1;
		iNew.fSize = 0.0f;
		iNew.vPos = vBest;
		CVector3 vDir = cCam.vView2 - cCam.vEye2;
		pair<CVector3, CVector3> vCDirs = getNormQuad(vDir);

		iNew.vStart = cCam.vView2 + vDir * 1.7f + vCDirs.first * 0.5f + vCDirs.second * 0.15f;
		if(iLIF != -1)
		{
			CVector3 vNorm = lv1.vNorms[iLIF];
			pair<CVector3, CVector3> vDirs = getNormQuad(vNorm);
			iNew.vDirA = vDirs.first;
			iNew.vDirB = vDirs.second;
			GLfloat fPFODist = -vecDot(vNorm, lv1.vFaces[iLIF*3]);
			GLfloat fPDist = vecDot(vNorm, cCam.vEye) + fPFODist;
			GLfloat fSign = fPDist > 0.0f ? 0.1f : -0.1f;
			iNew.vPos = vBest + vNorm * fSign;
		}
		iImpact.push_back(iNew);
		FSOUND_PlaySound(FSOUND_FREE, sndData[11]);
		fTime = 0.6f;
		iAmmo--;
		mWeapon.startAnimation("shoot", 1, &adWeapon);
		FOR(i, ESZ(swWebs))
		FOR(j, ESZ(swWebs[i].sbBalls))
		if(collisionSphereLine(swWebs[i].sbBalls[j].vPos, 2.0f, vEye2, vBest))
		{
			CVector3 vDir = cCam.vView2 - cCam.vEye2;
			vecNormalize(vDir);
			swWebs[i].sbBalls[j].vAcc += vDir * 100.0f;
		}
		FOR(j, ESZ(gCannon.bBalls))
		if(collisionSphereLine(gCannon.bBalls[j].vPos, 3.0f, vEye2, vBest))
			gCannon.bBalls[j].vAcc += vDir * 200.0f;
	}
}

//====================

GLvoid CShockRifle::renderBlended()
{
	if(fTime > 0.0f)fTime -= glAp.sof(1.0f);
	if(fTime < 0.0f)fTime = 0.0f;

	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uiExpTex);
	glBegin(GL_QUADS);
	glColor3ub(128, 0, 255);
	FOR(i, ESZ(iImpact))
	{
		if(iImpact[i].cOnlyBeam)continue;
		CVector3 vPoints[4];
		vPoints[0] = iImpact[i].vPos - iImpact[i].vDirA * iImpact[i].fSize + iImpact[i].vDirB * iImpact[i].fSize;
		vPoints[1] = iImpact[i].vPos + iImpact[i].vDirA * iImpact[i].fSize + iImpact[i].vDirB * iImpact[i].fSize;
		vPoints[2] = iImpact[i].vPos + iImpact[i].vDirA * iImpact[i].fSize - iImpact[i].vDirB * iImpact[i].fSize;
		vPoints[3] = iImpact[i].vPos - iImpact[i].vDirA * iImpact[i].fSize - iImpact[i].vDirB * iImpact[i].fSize;
		CVector2 vCoords[4];
		vCoords[0] = CVector2(0.0f, 1.0f);
		vCoords[1] = CVector2(1.0f, 1.0f);
		vCoords[2] = CVector2(1.0f, 0.0f);
		vCoords[3] = CVector2(0.0f, 0.0f);
		FOR(j, 4)
		{
			glTexCoord2f(vCoords[j].x, vCoords[j].y);
			glVertex3f(vPoints[j].x, vPoints[j].y, vPoints[j].z);
		}
	}
	glEnd();
	glLineWidth(5.0f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	FOR(i, ESZ(iImpact))
	{
		glColor4ub(128, 0, 255, (GL_UNSIGNED_BYTE)((10.0f - iImpact[i].fSize) / 10.0f * 255));
		glVertex3f(iImpact[i].vStart.x, iImpact[i].vStart.y, iImpact[i].vStart.z);
		glVertex3f(iImpact[i].vPos.x, iImpact[i].vPos.y, iImpact[i].vPos.z);
		iImpact[i].fSize += glAp.sof(20.0f);
		if(iImpact[i].fSize > 10.0f)
		{
			iImpact.erase(iImpact.begin() + i);
			i--;
		}
	}
	glEnd();
	glLineWidth(1.0f);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}