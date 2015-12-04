#include "mainheader.h"

#define WEIGHT 14.0f

CCannon gCannon;

//====================

void CCannon::renderWeapon()
{
	glTranslatef((float)sin(fShake) * 0.03f, 0.0f, fBack);
	mWeapon.renderModel(NULL);
}

//====================

void CCannon::renderAmmo()
{
	if(iCurWeapon != CANNON)fBack = 0.0f;
	FOR(i, ESZ(bBalls))
	{
		glPushMatrix();
		glTranslatef(bBalls[i].vPos.x, bBalls[i].vPos.y, bBalls[i].vPos.z);
		if(!bBalls[i].bExploding)
		{
			glDisable(GL_TEXTURE_2D);
			glColor3ub(32, 32, 32);
			GLUquadric *qObj = gluNewQuadric();
			gluSphere(qObj, 1.5f, 7, 7);
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glColor3ub(255, 128, 0);
			glBindTexture(GL_TEXTURE_2D, uiExpTex);
			glBegin(GL_QUADS);
			float fSize = bBalls[i].fExpSize;
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-fSize, 0.0f, -fSize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(fSize, 0.0f, -fSize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(fSize, 0.0f, fSize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-fSize, 0.0f, fSize);
			glEnd();
			glDisable(GL_BLEND);
		}
		glPopMatrix();
		if(!bBalls[i].bExploding)
		{
			bBalls[i].vAcc.y -= glAp.sof(WEIGHT * 7.0f);
			CVector3 vAdd = bBalls[i].vAcc * glAp.sof(1.0f);
			float fLen = vecMagnitude(vAdd);
			int iTimes = (int)(fLen / 1.5f) + 1;
			if(iTimes > 5)iTimes = 5;
			float fLen2 = vecMagnitude(bBalls[i].vAcc);
			bool bStand = true;
			FOR(l, iTimes)
			{
				bBalls[i].vLastPos = bBalls[i].vPos;
				bBalls[i].vPos += vAdd / (float)iTimes;
				FOR(j, ESZ(tTowers))FOR(k, tTowers[j].iNumCB)
				{
					float fDFCenter = 0.0f;
					if(collisionSpherePoly(tTowers[j].vColBox[k], 3, NULL, bBalls[i].vPos, 1.5f, fDFCenter))
					{
						float fAmplifier = vecMagnitude(bBalls[i].vAcc) / 90.0f;
						if(fAmplifier > 1.0f)fAmplifier = 1.0f;
						bBalls[i].fDamage *= fAmplifier;
						float fDMTaken = min(bBalls[i].fDamage, tTowers[j].fLife);
						bBalls[i].fDamage -= fDMTaken;
						tTowers[j].fLife -= fDMTaken;
					}
				}
				FOR(j, ESZ(sSpiders))
				{
					if(sSpiders[j].fLife < -25.0f)continue;
					CVector3 vPos = sSpiders[j].vPos; vPos.y += 1.0f;
					float fDist = vecDist(bBalls[i].vPos, vPos);
					if(fDist < 6.0f)
					{
						float fAmplifier = vecMagnitude(bBalls[i].vAcc) / 90.0f;
						if(fAmplifier > 1.0f)fAmplifier = 1.0f;
						bBalls[i].fDamage *= fAmplifier;
						float fDMTaken = min(bBalls[i].fDamage, sSpiders[j].fLife + 25.1f);
						bBalls[i].fDamage -= fDMTaken / 2.0f;
						sSpiders[j].fLife -= fDMTaken;
						sSpiders[j].addBlood(sSpiders[j].vPos, fDMTaken);
					}
				}

				FOR(k, ESZ(lv1.iStand))
				{
					int j = lv1.iStand[k];
					CVector3 vPoly[] = {lv1.vFaces[j*3], lv1.vFaces[j*3 + 1], lv1.vFaces[j*3 + 2]};
					float fDFCenter = 0.0f;
					if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[j], bBalls[i].vPos, 1.5f, fDFCenter))
					{
						CVector3 vOffset = lv1.vNorms[j];
						if(fDFCenter > 0.0f)vOffset *= (1.5f - fDFCenter);
						else vOffset *= (1.5f + fDFCenter) * -1;
						bBalls[i].vPos += vOffset;
						bBalls[i].vAcc = lv1.vNorms[j] * 2.0f * vecDot(bBalls[i].vAcc * -1.0f, lv1.vNorms[j]) + bBalls[i].vAcc;
						bBalls[i].vAcc.y *= 10.0f / WEIGHT;
						if(bBalls[i].vAcc.y < 1.1f && lv1.fAngles[j] < 4.0f * PI / 180.0f)bStand = true;
					}
				}

				FOR(k, ESZ(lv1.iNotStand))
				{
					int j = lv1.iNotStand[k];
					CVector3 vPoly[] = {lv1.vFaces[j*3], lv1.vFaces[j*3 + 1], lv1.vFaces[j*3 + 2]};
					float fDFCenter = 0.0f;
					if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[j], bBalls[i].vPos, 1.5f, fDFCenter))
					{
						CVector3 vOffset = lv1.vNorms[j];
						if(fDFCenter > 0.0f)vOffset *= (1.5f - fDFCenter);
						else vOffset *= (1.5f + fDFCenter) * -1;
						bBalls[i].vPos += vOffset;
						float fLen1 = vecMagnitude(bBalls[i].vAcc);
						CVector3 vNorm = lv1.vNorms[j];
						if(fDFCenter < 0.0f)vNorm *= -1;
						bBalls[i].vAcc = vNorm * 2.0f * vecDot(bBalls[i].vAcc * -1.0f, vNorm) + bBalls[i].vAcc;
						float fLen2 = vecMagnitude(bBalls[i].vAcc);
					}
				}
			}

			if((bBalls[i].vAcc.y < 2.0f && bBalls[i].vAcc.y >= -2.0f) && bStand)
			{
				float fLen = vecMagnitude(bBalls[i].vAcc);
				if(fLen != 0.0f)
				{
					float fSlowdown = WEIGHT * 4.0f;
					float fLen2 = fLen - glAp.sof(fSlowdown);
					float fRatio = fLen2 / fLen;
					if(fRatio < 0.0f)fRatio = 0.0f;
					bBalls[i].vAcc *= fRatio;
				}
			}

			FOR(j, ESZ(bBalls))
			{
				if(i != j && !bBalls[j].bExploding)
				{
					float fDist = vecDist(bBalls[i].vPos, bBalls[j].vPos);
					if(fDist < 3.0f)
					{
						bBalls[i].vPos = bBalls[i].vLastPos;
						bBalls[j].vPos = bBalls[j].vLastPos;
						CVector3 vNewDirA = bBalls[j].vPos - bBalls[i].vPos;
						vecNormalize(vNewDirA);
						bBalls[j].vAcc = vNewDirA * (vecMagnitude(bBalls[j].vAcc) / 2.0f + vecMagnitude(bBalls[i].vAcc) / 2.0f);
						bBalls[i].vAcc = vNewDirA * 2.0f * vecDot(bBalls[i].vAcc * -1.0f, vNewDirA) + bBalls[i].vAcc;
						bBalls[i].vAcc /= 2.0f;
					}
				}
			}
		}
		if(bBalls[i].fDamage <= 0.0f && bBalls[i].fLife > 0.0f)bBalls[i].fLife = -1.0f;
		if(bBalls[i].fLife > 0.0f)bBalls[i].fLife -= glAp.sof(1.0f);
		if(bBalls[i].fLife < 0.0f)
		{
			bBalls[i].bExploding = true;
			bBalls[i].fLife = 0.0f;
			if(vecDist(bBalls[i].vPos, cCam.vEye) < 80.0f)FSOUND_PlaySound(FSOUND_FREE, sndData[13]);
		}
		if(bBalls[i].fLife == 0.0f)bBalls[i].fExpSize += glAp.sof(30.0f);
		if(bBalls[i].fExpSize > 50.0f)
		{
			bBalls.erase(bBalls.begin() + i);
			i--;
		}
	}
	glEnable(GL_TEXTURE_2D);
	if(!bRecover)
	{
		if((!glAp.key(VK_LBUTTON) && fBack > 0.0f) || fBack == 1.5f)
		{
			bRecover = true;
			CCannonBall bNew;
			CVector3 vEye2 = cCam.vEye, vView2 = cCam.vView;
			vEye2.y += 3.0f; vView2.y += 3.0f;
			bNew.vAcc = vView2 - vEye2;
			vecNormalize(bNew.vAcc);
			float fLen = 55.0f;
			fLen += fBack * 143.3f;
			bNew.vPos = vEye2 + bNew.vAcc * 3.5f;
			bNew.vAcc *= fLen;
			bNew.fLife = 10.0f;
			bBalls.push_back(bNew);
			FSOUND_PlaySound(FSOUND_FREE, sndData[7]);
			if(fBack < 0.5f)fBack += 1.0f;
			iAmmo--;
		}
	}
	if(bRecover)
	{
		fTime += glAp.sof(1.0f);
		if(fTime > 0.8f)
		{
			fTime = 0.0f;
			bRecover = false;
		}
		if(fBack > 0.0f)
		{
			fBack -= glAp.sof(2.5f);
			if(fBack < 0.0f)fBack = 0.0f;
		}
	}
	FOR(i, ESZ(bBalls))
	{
		if(vecDist(bBalls[i].vPos, cCam.vEye2) < 6.0f && bBalls[i].fLife < 9.8f)
		{
			cCam.vAcc += bBalls[i].vAcc / 10.0f;
			bBalls[i].fLife = 0.0f;
			bBalls[i].bExploding = true;
		}
	}
}

//====================

void CCannon::shoot()
{
	if(!bRecover && iAmmo > 0)
	{
		if(fBack == 0.0f)FSOUND_PlaySound(FSOUND_FREE, sndData[9]);
		fBack += glAp.sof(1.5f);
		fShake += glAp.sof(fBack) * 50.0f;
	}
	if(fBack > 1.5f)fBack = 1.5f;
}

//====================

void CCannon::renderBlended(){}