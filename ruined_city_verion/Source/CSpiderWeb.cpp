#include "mainheader.h"

UINT uiSWTex;
CVector2 vCCoord[4] = // Classic coordinates
{
	CVector2(0.0f, 1.0f),
	CVector2(1.0f, 1.0f),
	CVector2(1.0f, 0.0f),
	CVector2(0.0f, 0.0f)
};

vector<CSpiderWeb> swWebs;

void CSpiderWeb::render()
{
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(255, 255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, uiSWTex);
	glBegin(GL_QUADS);
	FOR(i, 4){glTexCoord2f(vCCoord[i].x, vCCoord[i].y); glVertex3f(vPoints[i].x, vPoints[i].y, vPoints[i].z);}
	glEnd();
	glDisable(GL_BLEND);
	FOR(i, iNests)
	{
		float fScale = 0.25f + ((float)iNSpiders[i] / iNAll[i] * 0.75f);
		if(fScale == 0.25f)continue;
		fScale *= 1.5f;
		glPushMatrix();
		glTranslatef(vNests[i].x, vNests[i].y, vNests[i].z);
		glScalef(fScale, fScale, fScale);
		mEnemies[iType == 0 ? 3 : 6].renderModel(NULL);
		glPopMatrix();
	}
	FOR(i, ESZ(sbBalls))
	{
		sbBalls[i].fTime += glAp.sof(1.0f);
		if(sbBalls[i].fTime > 1.0f)
		{
			CSpider sNew;
			sNew.vPos = sbBalls[i].vPos;
			sNew.iType = iType == 0 ? 2 : 5;
			sNew.fLife = sNew.iType == 2 ? 50.0f : 75.0f;
			sSpiders.push_back(sNew);
			sbBalls.erase(sbBalls.begin() + i);
			i--;
			continue;
		}
		glPushMatrix();
		glTranslatef(sbBalls[i].vPos.x, sbBalls[i].vPos.y, sbBalls[i].vPos.z);
		CVector3 vDir = sbBalls[i].vAcc; vDir.y = 0.0f;
		float fAngle = (float)vecAngle(CVector3(0, 0, -1), vDir) * 180 / PI;
		glRotatef(fAngle, 0, 1, 0);
		glRotatef(sbBalls[i].fAngle, 1, 0, 0);
		sbBalls[i].fAngle += glAp.sof(270.0f);
		mEnemies[iType == 0 ? 4 : 7].renderModel(NULL);
		glPopMatrix();
		sbBalls[i].vPos += sbBalls[i].vAcc * glAp.sof(1.0f);
		sbBalls[i].vAcc.y -= glAp.sof(50.0f);
		FOR(k, ESZ(lv1.iStand))
		{
			int j = lv1.iStand[k];
			CVector3 vPoly[] = {lv1.vFaces[j*3], lv1.vFaces[j*3 + 1], lv1.vFaces[j*3 + 2]};
			float fDFCenter = 0.0f;
			if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[j], sbBalls[i].vPos, 1.5f, fDFCenter))
			{
				CVector3 vOffset = lv1.vNorms[j];
				if(fDFCenter > 0.0f)vOffset *= (1.5f - fDFCenter);
				else vOffset *= (1.5f + fDFCenter) * -1;
				sbBalls[i].vPos += vOffset;
				sbBalls[i].vAcc = lv1.vNorms[j] * 2.0f * vecDot(sbBalls[i].vAcc * -1.0f, lv1.vNorms[j]) + sbBalls[i].vAcc;
				sbBalls[i].vAcc.y *= 0.5f;
				if(sbBalls[i].vAcc.y < 1.1f && lv1.fAngles[j] < 4.0f * PI / 180.0f)sbBalls[i].vAcc.y = 0.0f;
			}
		}
		FOR(k, ESZ(lv1.iNotStand))
		{
			int j = lv1.iNotStand[k];
			CVector3 vPoly[] = {lv1.vFaces[j*3], lv1.vFaces[j*3 + 1], lv1.vFaces[j*3 + 2]};
			float fDFCenter = 0.0f;
			if(collisionSpherePoly(vPoly, 3, &lv1.vNorms[j], sbBalls[i].vPos, 1.5f, fDFCenter))
			{
				CVector3 vOffset = lv1.vNorms[j];
				if(fDFCenter > 0.0f)vOffset *= (1.5f - fDFCenter);
				else vOffset *= (1.5f + fDFCenter) * -1;
				sbBalls[i].vPos += vOffset;
				CVector3 vNorm = lv1.vNorms[j];
				sbBalls[i].vAcc = vNorm * 2.0f * vecDot(sbBalls[i].vAcc * -1.0f, vNorm) + sbBalls[i].vAcc;
			}
		}
	}
	if(iAll > 0)fTime += glAp.sof(1.0f);
	if(fTime > 1.5f && vecDist(vPoints[0], cCam.vEye) < 300.0f)
	{
		fTime = 0.0f;
		int iNest = rand() % iNests;
		if(iNSpiders[iNest] == 0)
		{
			iNest = 0;
			while(!iNSpiders[iNest]){iNest++; if(iNest == iNests)break;}
		}
		if(iNest != iNests)
		{
			iNSpiders[iNest]--;
			iAll--;
			CSpiderBall bNew;
			bNew.fAngle = 0.0f;
			bNew.vPos = vNests[iNest];
			bNew.fTime = 0.0f;
			CVector3 vDir = cCam.vEye2 - bNew.vPos;
			vecNormalize(vDir);
			vDir.x += -0.2f + (float)(rand() % 40) / 100.0f;
			vDir.y += -0.2f + (float)(rand() % 40) / 100.0f;
			vDir.z += -0.2f + (float)(rand() % 40) / 100.0f;
			bNew.vAcc = vDir * 50.0f;
			sbBalls.push_back(bNew);
		}
	}
}