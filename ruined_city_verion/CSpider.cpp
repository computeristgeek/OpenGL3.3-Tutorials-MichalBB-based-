#include "mainheader.h"

GLvoid CSpider::render()
{
	vLastPos = vPos;
	fGrav -= glAp.sof(0.5f);
	FOR(j, ESZ(lv1.iStand))
	{
		GLint i = lv1.iStand[j];
		CVector3 vPoly[] = {lv1.vFaces[i*3], lv1.vFaces[i*3 + 1], lv1.vFaces[i*3 + 2]};
		GLfloat fDFCenter = 0.0f;
		if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[i], vPos, 3.0f, fDFCenter))
		{
			CVector3 vOffset = lv1.vNorms[i];
			if(fDFCenter > 0)vOffset *= (3.0f - fDFCenter);
			else vOffset *= (3.0f + fDFCenter + 0.1f) * -1;
			vPos += vOffset;
			fGrav = 0.0f;
		}
	}
	vPos.y += fGrav;
	FOR(j, ESZ(lv1.iNotStand))
	{
		GLint i = lv1.iNotStand[j];
		CVector3 vPoly[] = {lv1.vFaces[i*3], lv1.vFaces[i*3 + 1], lv1.vFaces[i*3 + 2]};
		GLfloat fDFCenter = 0.0f;
		if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[i], vPos, 3.0f, fDFCenter))
		{
			CVector3 vOffset = lv1.vNorms[i];
			if(fDFCenter > 0)vOffset *= (3.0f - fDFCenter);
			else vOffset *= (3.0f + fDFCenter) * -1;
			vPos += vOffset;
		}
	}
	CVector3 vDir = cCam.vEye - vPos; vDir.y = 0.0f;
	vecNormalize(vDir);
	GLfloat fDist = vecDist(vPos, cCam.vEye);
	GLfloat fAngle2 = (float)vecAngle(CVector3(0, 0, -1), vDir) * 180 / PI;
	if(fLife > 0.0f)
	{
		if(fDist < 8.0f)
		{
			if(aData.iAnim == -1)
			{
				mEnemies[iType].startAnimation("attack", 1, &aData);
				bAttack = true;
				cCam.vAcc += vDir * (iType == 2 ? 60.0f : 100.0f);
			}
		}
		else
		{
			if(aData.iAnim == -1)mEnemies[iType].startAnimation("walk", 1, &aData);
			vPos += vDir * glAp.sof(25.0f);
		}
	}
	else
	{
		if(!bDying)
		{
			bDying = true;
			mEnemies[iType].startAnimation(fLife < -25.0f ? "die2" : "die", 1, &aData);
			FSOUND_PlaySound(FSOUND_FREE, sndData[15 + rand() % 2]);
			fDieAngleY = fAngle2;
			aData.bPauseAtEnd = true;
		}
	}
	if(cCam.vEye.x < vPos.x)fAngle2 = 360 - fAngle2;
	glTranslatef(0, -2, 0);
	glRotatef(bDying ? fDieAngleY : -fAngle2 - 180, 0, 1, 0);
	if(bDying)
	{
		if(fLife < -25.0f && aData.iAnim == 0)
		{
			mEnemies[iType].startAnimation("die2", 1, &aData);
			aData.bPauseAtEnd = true;
			fDieAngleX = 0.0f;
		}
		glRotatef(fDieAngleX, 1, 0, 0);
		if(fDieAngleX < 180.0f)
		{
			fDieAngleX += glAp.sof(540.0f);
			if(fDieAngleX > 180.0f)fDieAngleX = 180.0f;
		}
		else fFade -= glAp.sof(0.25f);
		glTranslatef(0, -fDieAngleX / 90.0f, 0);
	}
	GLfloat fScale = iType == 2 ? 2.0f : 2.5f;
	glScalef(fScale, fScale, fScale);
	mEnemies[iType].setGlobalBlend(fFade);
	mEnemies[iType].renderModel(&aData);
}

GLvoid CSpider::renderBlood()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	FOR(i, ESZ(vBAcc))
	{
		glColor3ub(0, 128, 0);
		glVertex3f(vBPos[i].x, vBPos[i].y, vBPos[i].z);
		glColor3ub(255, 255, 255);
		glVertex3f(vBPos[i].x + vBAcc[i].x * 2.0f, vBPos[i].y + vBAcc[i].y * 2.0f, vBPos[i].z + vBAcc[i].z * 2.0f);
		vBPos[i] += vBAcc[i] * glAp.sof(10.0f);
		vBAcc[i].y -= glAp.sof(5.0f);

		if(vBAcc[i].y < -1.0f)
		{
			vBPos.erase(vBPos.begin() + i);
			vBAcc.erase(vBAcc.begin() + i);
			i--;
		}
	}
	glEnd();
	glLineWidth(1.0f);
}

GLvoid CSpider::addBlood(CVector3 vPos, GLfloat fDamage)
{
	GLint iTimes = (int)fDamage;
	FOR(i, iTimes)
	{
		vBPos.push_back(vPos);
		CVector3 vDir;
		vDir.x += -1.0f + (float)(rand() % 201) / 100.0f;
		vDir.y += 0.5f + (float)(rand() % 51) / 100.0f;
		vDir.z += -1.0f + (float)(rand() % 201) / 100.0f;
		vecNormalize(vDir);
		vBAcc.push_back(vDir);
	}
}
