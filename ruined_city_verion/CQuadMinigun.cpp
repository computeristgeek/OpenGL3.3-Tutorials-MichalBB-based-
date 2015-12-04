#include "mainheader.h"

#define ROTSPEED 1080.0f

CQuadMinigun gQMinigun;

float fTrans[4][2] = 
{
	{0, 0.35f},
	{0, -0.35f},
	{-0.32f, 0.0f},
	{0.32f, 0.0f},
};

//====================

void CQuadMinigun::renderWeapon()
{
	if(!bLoaded)
	{
		bLoaded = true;
		fMSpeed = 0.0f;
		FOR(i, 4)fSpeed[i] = 0.0f;
	}
	glTranslatef(0.7f, -0.8f, -1.2f);
	fMAngle += glAp.sof(fMSpeed);
	glRotatef(fMAngle, 0, 0, -1);
	mWeapon.renderModel(NULL);
	FOR(i, 4)
	{
		glPushMatrix();
		glTranslatef(fTrans[i][0], fTrans[i][1], -0.1f);
		mRotator.renderModel(NULL);
		glTranslatef(0.0f, 0.0f, -0.3f);
		glRotatef(fAngle[i], 0, 0, 1);
		if(fSpeed[i] > 0.0f)fAngle[i] += glAp.sof(fSpeed[i]);
		mCannon.renderModel(&adFire[i]);
		glPopMatrix();
	}
	if(!glAp.key(VK_LBUTTON) || iAmmo == 0 || bOverheat)
	{
		fMSpeed -= glAp.sof(ROTSPEED / 4.0f);
		if(fMSpeed < 0.0f)fMSpeed = 0.0f; 
		FOR(i, 4)
		{
			fSpeed[i] -= glAp.sof(ROTSPEED);
			if(fSpeed[i] < 0.0f)
				fSpeed[i] = -480.0f * (float)i;
		}
		if(iPhase == 1)
		{
			FSOUND_StopSound(iChannel);
			FSOUND_PlaySound(iChannel, sndData[6]);
			iPhase--;
		}
	}
	if(fOverheat > 0.0f)
	{
		ortho2DBegin(glAp.scrX, glAp.scrY);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		if(fOverheat > 75.0f && !bOverheat)ft.print(CFData(15, 150, 30), "OVERHEATING!!!");
		if(bOverheat)ft.print(CFData(32, ft.getcenterx(ft.getxsize(FT("OVERHEATED!"), 32), 0, glAp.scrX), glAp.scrY / 2), "OVERHEATED!");
		glDisable(GL_BLEND);
		ortho2DEnd();
	}
}

//====================

void CQuadMinigun::renderAmmo()
{
	if(!glAp.key(VK_LBUTTON) || iCurWeapon != QMINIGUN || bOverheat)fOverheat -= glAp.sof(5.0f);
	if(bOverheat && fOverheat < 75.0f)bOverheat = false;
	if(fOverheat < 0.0f)fOverheat = 0.0f;
	if(iCurWeapon != QMINIGUN)
	{
		if(iChannel != -1)
		if(FSOUND_IsPlaying(iChannel))FSOUND_StopSound(iChannel);
		bLoaded = false;
		iAmmo = gDUzi.iAmmo;
		FOR(i, 4)fSpeed[i] = -480.0f * (float)i;
	}
}

//====================

void CQuadMinigun::shoot()
{
	if(iPhase == 0 && !bOverheat && iAmmo != 0)
	{
		iChannel = FSOUND_PlaySound(FSOUND_FREE, sndData[5]);
		iPhase++;
	}
	if(iPhase == 1 && !FSOUND_IsPlaying(iChannel))
	{
		FSOUND_PlaySound(iChannel, sndData[4]);
		FSOUND_SetLoopMode(iChannel, FSOUND_LOOP_NORMAL);
	}
	if(iAmmo > 0 && !bOverheat)
	{
		fMSpeed += glAp.sof(ROTSPEED / 4.0f);
		if(fMSpeed > 270.0f)fMSpeed = 270.0f;
		FOR(i, 4)
		{
			fSpeed[i] += glAp.sof(ROTSPEED);
			if(fSpeed[i] > 1080.0f)fSpeed[i] = 1080.0f;
			if(fSpeed[i] == 1080.0f)
			{
				fTime[i] += glAp.sof(1.0f);
				if(fTime[i] > 0.15f)
				{
					fTime[i] = 0.0f;
					mCannon.startAnimation("fire", 1, &adFire[i]);
					FSOUND_PlaySound(FSOUND_FREE, sndData[3]);
					iAmmo--;
					fOverheat += 0.25f;
					if(fOverheat >= 100.0f)
					{
						fOverheat = 100.0f;
						bOverheat = true;
					}

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
					CVector3 vFPos = cCam.vView2 + vDir * 1.8f + vCDirs.first * 0.5f + vCDirs.second * 0.4f;
					vFPos += vCDirs.first * (-0.25f + (float)(rand() % 500) / 1000.0f);
					vFPos += vCDirs.second * (-0.25f + (float)(rand() % 500) / 1000.0f);
					CVector3 vDir2 = vTarget - vFPos;
					vecNormalize(vDir2);
					CBullet bNew;
					bNew.vDir = vDir2;
					bNew.vPos = vFPos;
					bNew.vEnd = vTarget;
					bNew.fLife = 1.0f;
					gDUzi.bBullets.push_back(bNew);
					if(bOverheat || iAmmo == 0)break;
				}
			}
		}
	}
}

//====================

void CQuadMinigun::renderBlended(){}