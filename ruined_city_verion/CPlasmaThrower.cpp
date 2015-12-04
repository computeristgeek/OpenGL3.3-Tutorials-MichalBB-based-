#include "mainheader.h"

CPlasmaThrower gPlasmaGun;

//====================

GLvoid CPlasmaThrower::renderWeapon()
{
	if(iPhase == 1)
	{
		fUp += glAp.sof(1.0f);
		if(fUp > 0.08f){fUp = 0.08f; iPhase++;}
	}
	else if(iPhase == 2)
	{
		fUp -= glAp.sof(1.0f);
		if(fUp < 0.0f){fUp = 0.0f; iPhase = 0;}
	}
	if(iPhase != 0)glTranslatef(0.0f, fUp * 0.7f, fUp * 3.0f);
	mWeapon.renderModel(NULL);
}

//====================

GLvoid CPlasmaThrower::renderAmmo()
{
	glDisable(GL_TEXTURE_2D);
	glColor3ub(0, 64, 128);
	//glBegin(GL_QUADS);
	FOR(i, ESZ(pPlasmas))
	{
		GLfloat sine = sin(pPlasmas[i].fAngle * PI / 180.0f);
		GLfloat cosine = cos(pPlasmas[i].fAngle * PI / 180.0f);
		
		CVector3 vPos = pPlasmas[i].vPos
			+ pPlasmas[i].vUp * cosine * 0.5f
			+ pPlasmas[i].vRight * sine * 0.5f;/*
		CVector3 vPoint[4];
		vPoint[0] = vPos - pPlasmas[i].vRight * 0.5f + pPlasmas[i].vUp * 0.5f;
		vPoint[1] = vPos + pPlasmas[i].vRight * 0.5f + pPlasmas[i].vUp * 0.5f;
		vPoint[2] = vPos + pPlasmas[i].vRight * 0.5f - pPlasmas[i].vUp * 0.5f;
		vPoint[3] = vPos - pPlasmas[i].vRight * 0.5f - pPlasmas[i].vUp * 0.5f;
		FOR(j, 4)glVertex3f(vPoint[j].x, vPoint[j].y, vPoint[j].z);
		*/
		glPushMatrix();
		glTranslatef(vPos.x, vPos.y, vPos.z);
		GLUquadric *qObj = gluNewQuadric();
		gluSphere(qObj, 0.5f, 4, 4);
		glPopMatrix();
		
		pPlasmas[i].vLastPos = vPos;
		pPlasmas[i].vPos += pPlasmas[i].vDir * glAp.sof(300.0f);
		pPlasmas[i].fAngle += glAp.sof(720.0f);
		pPlasmas[i].fLife -= glAp.sof(1.0f);
		CVector3 vDif1 = pPlasmas[i].vLastPos - pPlasmas[i].vEnd;
		CVector3 vDif2 = pPlasmas[i].vPos - pPlasmas[i].vEnd;

		GLint iCollide = collisionLineEnemies(pPlasmas[i].vLastPos, pPlasmas[i].vPos);
		if(vDif1.x * vDif2.x < 0.0f || vDif1.y * vDif2.y < 0.0f || vDif1.z * vDif2.z < 0.0f || pPlasmas[i].fLife < 0.0f || iCollide)
		{
			pPlasmas.erase(pPlasmas.begin() + i);
			if(iCollide)takeEnemiesLife(20.0f);
		}
	}
	//glEnd();
	glEnable(GL_TEXTURE_2D);
}

//====================

GLvoid CPlasmaThrower::shoot()
{
	if(iPhase == 0 && iAmmo != 0)
	{
		iPhase++;
		iAmmo--;
		FSOUND_PlaySound(FSOUND_FREE, sndData[17]);
		CPlasma pNew;
		pNew.fAngle = 0.0f;
		pNew.fLife = 10.0f;
		pNew.vEnd = getIntersection(cCam.vEye2, cCam.vView2);
		CVector3 vDir = cCam.vView2 - cCam.vEye2;
		vecNormalize(vDir);
		pNew.vPos = cCam.vView2 + vDir * 2.7f + cCam.vQuadA * 0.7f + cCam.vQuadB * 0.15f;
		pNew.vLastPos = pNew.vPos;
		pNew.vDir = pNew.vEnd - pNew.vPos;
		vecNormalize(pNew.vDir);
		pair<CVector3, CVector3> vQuad = getNormQuad(pNew.vDir);
		pNew.vUp = vQuad.second;
		pNew.vRight = vQuad.first;
		pPlasmas.push_back(pNew);
	}
}

//====================

GLvoid CPlasmaThrower::renderBlended()
{
}

GLvoid CLightingGun::renderBlended(){}