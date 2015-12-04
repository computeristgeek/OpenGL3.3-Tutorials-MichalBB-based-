#include "mainheader.h"

void CTower::render()
{
	BYTE bColor = (BYTE)(255.0f - (255.0f * (fLife / 100.0f)));
	if(bColor > 128 && fLife > 0.0f)
	{
		fTime2 += glAp.sof(1.0f);
		if(fTime2 > 0.33f)
		{
			fTime2 = 0.0f;
			CSmoke sNew;
			sNew.bColor = bColor;
			sNew.vPos = vPos;
			sNew.fLife = 2.0f;
			sSmoke.push_back(sNew);
		}
	}
	if(fLife <= 0.0f)
	{
		if(bPlaySnd)
		{
			bPlaySnd = false;
			FSOUND_PlaySound(FSOUND_FREE, sndData[12]);
		}
		float fAdd = glAp.sof(10.0f);
		vPos.y += fAdd;
		FOR(i, iNumCB)FOR(j, iNumPoints[i])vColBox[i][j].y += fAdd;
		fFade -= glAp.sof(1.0f);
		fAngle += glAp.sof(720.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		
		glColor3ub(255, 128, 0);
		glBindTexture(GL_TEXTURE_2D, uiExpTex);
		float fSize = (1.0f - fFade) * 25.0f;
		pair<CVector3, CVector3> vDirs = getNormQuad(cCam.vView - cCam.vEye);
		CVector3 vPoints[4];
		vPoints[0] = vDirs.first * fSize * (-1) + vDirs.second * fSize;
		vPoints[1] = vDirs.first * fSize + vDirs.second * fSize;
		vPoints[2] = vDirs.first * fSize - vDirs.second * fSize;
		vPoints[3] = vDirs.first * fSize * (-1) - vDirs.second * fSize;
		CVector2 vCoords[4];
		vCoords[0] = CVector2(0.0f, 1.0f);
		vCoords[1] = CVector2(1.0f, 1.0f);
		vCoords[2] = CVector2(1.0f, 0.0f);
		vCoords[3] = CVector2(0.0f, 0.0f);
		glBegin(GL_QUADS);
		FOR(j, 4)
		{
			glTexCoord2f(vCoords[j].x, vCoords[j].y);
			glVertex3f(vPoints[j].x, vPoints[j].y, vPoints[j].z);
		}
		glEnd();
		glDisable(GL_BLEND);
		glRotatef(fAngle, 1, 0, 0);
	}
	mEnemies[0].setGlobalBlend(fFade);
	mEnemies[0].renderModel(NULL);
	glTranslatef(0.0f, 3.5f, 0.0f);
	CVector3 vDir = cCam.vEye - vPos; vDir.y = 0.0f;
	float fAngle2 = (float)vecAngle(CVector3(0, 0, -1), vDir) * 180 / PI;
	if(cCam.vEye.x < vPos.x)fAngle2 = 360 - fAngle2;
	
	if(fabsf(fAngle - fAngle2) > 3.0f)bRotating = true;
	if(bRotating)
	{
		float fCW = fAngle2 > fAngle ? fAngle2 - fAngle : 360.0f - fAngle + fAngle2;
		float fCCW = fAngle2 < fAngle ? fAngle - fAngle2 : 360.0f - fAngle2 + fAngle;
		float fDifA = fAngle - fAngle2;
		if(fCW < fCCW)fAngle += glAp.sof(90.0f);
		else fAngle -= glAp.sof(90.0f);
		float fDifB = fAngle - fAngle2;
		if(fAngle < 0.0f)fAngle += 360.0f;
		if(fAngle > 360.0f)fAngle -= 360.0f;
		if(fDifA * fDifB < 0.0f){fAngle = fAngle2; bRotating = false;}
	}
	else
	{
		float fDist = vecDist(cCam.vEye2, vPos);
		if(fTime == 0.0f && fDist < 400.0f)
		{
			CVector3 vPos2 = vPos; vPos2.y += 5.5f;
			CVector3 vDir = cCam.vEye2 - vPos2;
			float s = vDir.y;
			vDir.y = 0.0f;
			float fDist = vecMagnitude(vDir);
			float v = 200.0f;
			float t = max(fDist / v, s / v);
			float a = 14.0f * 7.0f;
			CCannonBall bNew;
			bNew.fLife = 10.0f;
			bNew.vPos = vPos2;
			vecNormalize(vDir);
			bNew.vAcc = vDir * 200.0f;
			bNew.vAcc.y = (2*s + a*t*t) / (2*t);
			gCannon.bBalls.push_back(bNew);
			fTime = 3.0f;
			FSOUND_PlaySound(FSOUND_FREE, sndData[14]);
		}
	}
	if(fTime > 0.0f)
	{
		fTime -= glAp.sof(1.0f);
		if(fTime < 0.0f)fTime = 0.0f;
	}
	
	glRotatef(-fAngle, 0, 1, 0);
	mEnemies[1].setGlobalBlend(fFade);
	mEnemies[1].renderModel(NULL);
	if(fLife <= 0.0f)glDisable(GL_BLEND);
}

//====================

void CTower::renderSmoke()
{
	glBegin(GL_QUADS);
	float fSize = 2.0f;
	FOR(i, ESZ(sSmoke))
	{
		glColor4ub(sSmoke[i].bColor, sSmoke[i].bColor, sSmoke[i].bColor, (BYTE)((sSmoke[i].fLife / 2.0f) * 255.0f));
		CVector3 vPoints[4];
		vPoints[0] = sSmoke[i].vPos - cCam.vQuadA * fSize + cCam.vQuadB * fSize;
		vPoints[1] = sSmoke[i].vPos + cCam.vQuadA * fSize + cCam.vQuadB * fSize;
		vPoints[2] = sSmoke[i].vPos + cCam.vQuadA * fSize - cCam.vQuadB * fSize;
		vPoints[3] = sSmoke[i].vPos - cCam.vQuadA * fSize - cCam.vQuadB * fSize;
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
		sSmoke[i].vPos.y += glAp.sof(10.0f);
		sSmoke[i].fLife -= glAp.sof(1.0f);
		if(sSmoke[i].fLife < 0.0f)
		{
			sSmoke.erase(sSmoke.begin() + i);
			i--;
		}
	}
	glEnd();
}