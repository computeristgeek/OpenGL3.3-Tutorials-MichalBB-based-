#include "mainheader.h"

CFlakCannon gFCannon;

GLubyte ubColor[2][2] = 
{
	{64, 32},
	{255, 255},
};

#define FLAKLENGTH 15.0f
#define FLAKSPEED 350.0f

//====================

GLvoid CFlakCannon::renderWeapon()
{
	glTranslatef(0, 0, 0.2f);
	mWeapon.renderModel(&adWeapon);
}

//====================

GLvoid CFlakCannon::renderAmmo()
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	FOR(i, ESZ(fShells))
	{
		GLint j = 1 - fShells[i].iCurNode % 2;
		glColor4ub(ubColor[j][0], ubColor[j][1], 0, fShells[i].fLife > 1.0f ? 255 : (BYTE)(fShells[i].fLife * 255));
		glVertex3f(fShells[i].vPos.x, fShells[i].vPos.y, fShells[i].vPos.z);

		CVector3 vPos2 = fShells[i].vPos + fShells[i].vDir[fShells[i].iCurNode] * FLAKLENGTH;
		GLfloat fDist = vecDist(fShells[i].vPos, fShells[i].vNodes[fShells[i].iCurNode]);
		if(fDist < FLAKLENGTH)vPos2 = fShells[i].vNodes[fShells[i].iCurNode];
		glColor4ub(ubColor[1 - j][0], ubColor[1 - j][1], 0, fShells[i].fLife > 1.0f ? 255 : (BYTE)(fShells[i].fLife * 255));
		glVertex3f(vPos2.x, vPos2.y, vPos2.z);
		if(fDist < FLAKLENGTH && fShells[i].iCurNode + 1 != fShells[i].iAllNodes)
		{
			GLfloat fLen = FLAKLENGTH - fDist;
			j = fShells[i].iCurNode % 2;
			glColor4ub(ubColor[j][0], ubColor[j][1], 0, fShells[i].fLife > 1.0f ? 255 : (BYTE)(fShells[i].fLife * 255));
			glVertex3f(vPos2.x, vPos2.y, vPos2.z);
			vPos2 += fShells[i].vDir[fShells[i].iCurNode + 1] * fLen;
			glColor4ub(ubColor[1 - j][0], ubColor[1 - j][1], 0, fShells[i].fLife > 1.0f ? 255 : (BYTE)(fShells[i].fLife * 255));
			glVertex3f(vPos2.x, vPos2.y, vPos2.z);
		}

		CVector3 vDif1 = fShells[i].vPos - fShells[i].vNodes[fShells[i].iCurNode];
		fShells[i].vPos += fShells[i].vDir[fShells[i].iCurNode] * glAp.sof(FLAKSPEED);
		CVector3 vDif2 = fShells[i].vPos - fShells[i].vNodes[fShells[i].iCurNode];
		if(vDif1.x * vDif2.x < 0.0f || vDif1.y * vDif2.y < 0.0f || vDif1.z * vDif2.z < 0.0f)
		{
			fShells[i].vPos = fShells[i].vNodes[fShells[i].iCurNode];
			fShells[i].iCurNode++;
		}
		fShells[i].fLife -= glAp.sof(1.0f);

		GLint iCollide = collisionLineEnemies(fShells[i].vPos, fShells[i].vPos + fShells[i].vDir[fShells[i].iCurNode] * FLAKLENGTH);
		if(fShells[i].iCurNode == fShells[i].iAllNodes || fShells[i].fLife < 0.0f || iCollide)
		{
			fShells.erase(fShells.begin() + i);
			if(iCollide)takeEnemiesLife(10.0f);
		}
	}
	glEnd();
	glLineWidth(1.0f);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, uiPartTex);
	glColor3ub(255, 255, 0);
	glBegin(GL_QUADS);
	GLfloat fSize = 0.5f;
	pair<CVector3, CVector3> vDirs = getNormQuad(cCam.vView - cCam.vEye);
	FOR(i, ESZ(fShells))
	{
		CVector3 vPoints[4];
		CVector3 vPos = fShells[i].vPos + fShells[i].vDir[fShells[i].iCurNode] * FLAKLENGTH;
		vPoints[0] = vPos - vDirs.first * fSize + vDirs.second * fSize;
		vPoints[1] = vPos + vDirs.first * fSize + vDirs.second * fSize;
		vPoints[2] = vPos + vDirs.first * fSize - vDirs.second * fSize;
		vPoints[3] = vPos - vDirs.first * fSize - vDirs.second * fSize;
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
	glDisable(GL_BLEND);
}

//====================

GLvoid CFlakCannon::shoot()
{
	if(adWeapon.iAnim == -1 && iAmmo > 0)
	{
		mWeapon.startAnimation("shoot", 1, &adWeapon);
		CVector3 vDir = cCam.vView2 - cCam.vEye2;
		vecNormalize(vDir);
		pair<CVector3, CVector3> vCDirs = getNormQuad(vDir);
		FOR(i, 5)
		{
			FOR(j, 2)
			{
				CFlak fShell; fShell.iAllNodes = 0;
				CVector3 vCurPos = cCam.vEye2 + vCDirs.first * (float)(i), vCurDir = vDir;
				GLfloat fRange = -0.02f + (float)(rand() % 40) / 1000.0f;
				vCurDir += vCDirs.first * fRange;
				fRange = -0.02f + (float)(rand() % 40) / 1000.0f;
				vCurDir += vCDirs.second * fRange;
				vecNormalize(vCurDir);
				vCurPos += vCDirs.second;
				fShell.vPos = vCurPos;
				FOR(k, 5)
				{
					CVector3 vNode = getIntersection(vCurPos, vCurPos + vCurDir);
					fShell.vDir[k] = vCurDir;
					fShell.iAllNodes++;
					if(iLIF == -1)break;
					GLfloat fPFODist = -vecDot(lv1.vNorms[iLIF], lv1.vFaces[iLIF*3]);
					GLfloat fPDist = vecDot(lv1.vNorms[iLIF], vCurPos) + fPFODist;
					CVector3 vNorm = lv1.vNorms[iLIF];
					vCurDir = vNorm * 2.0f * vecDot(vCurDir * -1.0f, vNorm) + vCurDir;
					CVector3 vNodeFinal = vNode;
					if(fPDist < 0.0f)vNodeFinal -= vNorm * 0.01f;
					else vNodeFinal += vNorm * 0.01f;
					fShell.vNodes[k] = vNodeFinal;
					vCurPos = vNodeFinal;
				}
				fShell.iCurNode = 0;
				fShell.fLife = 5.0f;
				fShells.push_back(fShell);
			}
		}
		FSOUND_PlaySound(FSOUND_FREE, sndData[10]);
		iAmmo--;
	}
}

//====================

GLvoid CFlakCannon::renderBlended(){}