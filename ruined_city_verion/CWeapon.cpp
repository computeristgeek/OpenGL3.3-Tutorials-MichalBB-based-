#include "mainheader.h"

CWeapon* wWeapons[ALLWEAPONS];
CLightingGun gLGun;
GLfloat fPrintName = 2.0f;

GLint iLIF = -1; // Last intersected face
GLint iLIE = -1, iEnType; // Last intersected enemy, enemy type
GLint iCurWeapon = 0, iNextWeapon = 0;
GLboolean bChanging = false;
GLfloat fFar = 0.0f;

CVector3 getIntersectionEnemy(CVector3 vFrom, CVector3 vTo, CVector3 vBest)
{
	CVector3 vDir = vTo - vFrom;
	vecNormalize(vDir);
	CVector3 vNext = vFrom + vDir * 10000.0f;
	GLfloat fLowest = vecDist(vFrom, vBest);
	iLIE = -1;
	FOR(i, ESZ(tTowers))
	{
		FOR(j, tTowers[i].iNumCB)
		{
			if(collisionLinePoly(vFrom, vNext, NULL, tTowers[i].vColBox[j], tTowers[i].iNumPoints[j]))
			{
				GLfloat fDist = vecDist(getLastIntersection(), vFrom);
				if(fDist < fLowest)
				{
					fLowest = fDist;
					vBest = getLastIntersection();
					iLIE = i;
					iEnType = 0;
				}
			}
		}
	}
	FOR(i, ESZ(sSpiders))
	{
		if(sSpiders[i].fLife < -25.0f)continue;
		CVector3 vPos = sSpiders[i].vPos; if(!sSpiders[i].bDying)vPos.y += 1.0f;
		CVector3 vClosest = closestPointOnLine(vFrom, vNext, vPos);
		GLfloat fDist = vecDist(vClosest, vPos);
		if(fDist < 1.5f)
		{
			GLfloat fLen = 1.5f - fDist;
			CVector3 vPoGLint = vClosest - vDir * fLen;
			fDist = vecDist(vPoint, vFrom);
			if(fDist < fLowest)
			{
				fLowest = fDist;
				vBest = vPoint;
				iLIE = i;
				iEnType = 1;
			}
		}
	}
	return vBest;
}

CVector3 getIntersection(CVector3 vFrom, CVector3 vTo)
{
	CVector3 vDir = vTo - vFrom;
	vecNormalize(vDir);
	CVector3 vNext = vFrom + vDir * 10000.0f;
	CVector3 vBest = vNext;
	GLfloat fLowest = 10001.0f;
	iLIF = -1;
	FOR(j, lv1.iAll)
	{
		CVector3 vPoly[] = {lv1.vFaces[j*3], lv1.vFaces[j*3 + 1], lv1.vFaces[j*3 + 2]};
		if(collisionLinePoly(vFrom, vNext, &lv1.vNorms[j], vPoly, 3))
		{
			GLfloat fDist = vecDist(getLastIntersection(), vFrom);
			if(fDist < fLowest)
			{
				fLowest = fDist;
				vBest = getLastIntersection();
				iLIF = j;
			}
		}
	}
	return vBest;
}

GLvoid CLightingGun::renderWeapon()
{
	
}

GLvoid CLightingGun::renderAmmo(){}

GLvoid CLightingGun::shoot(){}

GLvoid loadWeapons()
{
	GLchar strDir[512], strCopy[512]; GetCurrentDirectory(512, strDir);
	strcpy(strCopy, strDir);
	strcat(strDir, "\\Weapons");
	SetCurrentDirectory(strDir);
	gLGun.mWeapon.loadModelData("lighting_gun_held.msm");

	gRipper.mRDisc.loadModelData("ripperdisc.msm");
	gRipper.mWeapon.loadModelData("ripper_held.msm");
	gRipper.iAmmo = 45;
	gRipper.strName = "Ripper";
	gRipper.bR = 200; gRipper.bG = 200; gRipper.bB = 200;

	gDUzi.mWeapon.loadModelData("uzi.msm");
	gDUzi.iAmmo = 1000;
	gDUzi.strName = "Double Uzis";
	gDUzi.bR = 255; gDUzi.bG = 255; gDUzi.bB = 255;

	gQMinigun.mWeapon.loadModelData("mg_part1.msm");
	gQMinigun.mRotator.loadModelData("mg_part2.msm");
	gQMinigun.mCannon.loadModelData("mg_part3.msm");
	gQMinigun.iAmmo = 1000;
	gQMinigun.strName = "Quad Minigun";
	gQMinigun.bR = 64; gQMinigun.bG = 64; gQMinigun.bB = 64;

	gCannon.mWeapon.loadModelData("cannon_held.msm");
	gCannon.iAmmo = 30;
	gCannon.strName = "Uranium Cannon";
	gCannon.bR = 222; gCannon.bG = 111; gCannon.bB = 111;

	gFCannon.mWeapon.loadModelData("flak_held.msm");
	gFCannon.iAmmo = 40;
	gFCannon.strName = "Flak Cannon";
	gFCannon.bR = 255; gFCannon.bG = 255; gFCannon.bB = 0;

	gShockRifle.mWeapon.loadModelData("shockrifle.msm");
	gShockRifle.iAmmo = 50;
	gShockRifle.strName = "Shock Launcher";
	gShockRifle.bR = 192; gShockRifle.bG = 0; gShockRifle.bB = 255; 

	gPlasmaGun.mWeapon.loadModelData("plasma_thrower_held.msm");
	gPlasmaGun.iAmmo = 400;
	gPlasmaGun.strName = "Plasma Thrower";
	gPlasmaGun.bR = 0; gPlasmaGun.bG = 64; gPlasmaGun.bB = 128;

	textureBMP(&uiExpTex, "Explosion.bmp", false, false, LINEAR);
	textureBMP(&uiPartTex, "Particle.bmp", false, false, LINEAR);
	textureBMP(&uiSmokeTex, "smoketex.bmp", false, false, LINEAR);

	SetCurrentDirectory(strCopy);
	wWeapons[DOUBLEUZI] = &gDUzi;
	wWeapons[RIPPER] = &gRipper;
	wWeapons[SHOCKRIFLE] = &gShockRifle;
	wWeapons[QMINIGUN] = &gQMinigun;
	wWeapons[FLAKCANNON] = &gFCannon;
	wWeapons[CANNON] = &gCannon;
	wWeapons[PLASMATHROWER] = &gPlasmaGun;

	textureBMP(&uiSWTex, "Textures\\web.bmp", false, true, LINEAR);
}

GLvoid processWeapons()
{
	glPushMatrix();
	GLfloat fMoveX = sin(cCam.fBobAn) * 0.05f,
		fMoveY = abs(cos(cCam.fBobAn) * 0.05f);
	glTranslatef(cCam.vEye.x, cCam.vEye.y + 3, cCam.vEye.z);
	
	glRotatef(-cCam.getAngleY(), 0, 1, 0);
	glRotatef(cCam.getAngleX(), 1, 0, 0);
	glTranslatef(fMoveX, fMoveY, 0);
	FOR(i, ALLWEAPONS)if(glAp.onekey('1' + i)){bChanging = true; iNextWeapon = i;}
	if(bChanging)
	{
		if(iCurWeapon == iNextWeapon)
		{
			fFar -= glAp.sof(3.0f);
			if(fFar < 0.0f){fFar = 0.0f; bChanging = false;}
		}
		else
		{
			fFar += glAp.sof(3.0f);
			if(fFar > 1.0f){fFar = 1.0f; iCurWeapon = iNextWeapon; fPrintName = 2.0f;}
		}
		glRotatef(60 * fFar, 1, 0, 0);
		glTranslatef(0, 0, fFar);
	}
	if(iCurWeapon != RIPPER && iCurWeapon != FLAKCANNON)glEnable(GL_CULL_FACE);
	wWeapons[iCurWeapon]->renderWeapon();
	if(iCurWeapon != RIPPER && iCurWeapon != FLAKCANNON)glDisable(GL_CULL_FACE);
	if(glAp.key(VK_LBUTTON) && !bChanging)wWeapons[iCurWeapon]->shoot();
	glPopMatrix();
	FOR(i, ALLWEAPONS)
	wWeapons[i]->renderAmmo();
}

GLvoid renderWeaponsOrtho()
{
	ortho2DBegin(glAp.scrX, glAp.scrY);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES); glColor3ub(255, 255, 255);
	glVertex2i(glAp.scrX / 2 - 10, glAp.scrY / 2); glVertex2i(glAp.scrX / 2 + 10, glAp.scrY / 2);
	glVertex2d(glAp.scrX / 2, glAp.scrY / 2 + 10); glVertex2d(glAp.scrX / 2, glAp.scrY / 2- 10);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLchar aa[112]; sprintf(aa, "%d", wWeapons[iCurWeapon]->iAmmo);
	ft.print(CFData(50, 40, 20), aa);
	
	if(fPrintName > 0.0f)
	{
		glColor4ub(wWeapons[iCurWeapon]->bR, wWeapons[iCurWeapon]->bG, wWeapons[iCurWeapon]->bB, (BYTE)(255.0f * min(1.0f, fPrintName)));
		ft.print(CFData(35, ft.getcenterx(ft.getxsize(FT((char*)wWeapons[iCurWeapon]->strName.c_str()), 35), 0, glAp.scrX), 50), (char*)wWeapons[iCurWeapon]->strName.c_str());
		fPrintName -= glAp.sof(1.0f);
	}
	glDisable(GL_BLEND);
	ortho2DEnd();
}

GLvoid renderBox(CVector3 vPos)
{
	glPushMatrix();
	
	static GLfloat angle = 0; angle += glAp.sof(90.0f) / (float)(ESZ(gRipper.rdDiscs) + 1);
	glTranslatef(vPos.x, vPos.y, vPos.z);
	glRotatef(angle, 1, 0, 0);
	glRotatef(angle, 0, 1, 0);
	glRotatef(angle, 0, 0, 1);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	// Pøední stìna
	
	glNormal3f( 0.0f, 0.0f, 1.0f); // Normála
	glColor3ub(255, 0, 0);
	glVertex3f(-0.4f, -0.4f, 0.4f);
	glVertex3f( 0.4f, -0.4f, 0.4f);
	glVertex3f( 0.4f, 0.4f, 0.4f);
	glVertex3f(-0.4f, 0.4f, 0.4f);

	// Zadní stìna

	glNormal3f( 0.0f, 0.0f,-1.0f);// Normála
	glColor3ub(0, 255, 0);
	glVertex3f(-0.4f, -0.4f, -0.4f);
	glVertex3f(-0.4f, 0.4f, -0.4f);
	glVertex3f( 0.4f, 0.4f, -0.4f);
	glVertex3f( 0.4f, -0.4f, -0.4f);

	// Horní stìna

	glNormal3f( 0.0f, 1.0f, 0.0f);// Normála
	glColor3ub(0, 0, 255);
	glVertex3f(-0.4f, 0.4f, -0.4f);
	glVertex3f(-0.4f, 0.4f, 0.4f);
	glVertex3f( 0.4f, 0.4f, 0.4f);
	glVertex3f( 0.4f, 0.4f, -0.4f);

	// Spodní stìna

	glNormal3f( 0.0f,-1.0f, 0.0f);// Normála
	glColor3ub(255, 0, 255);
	glVertex3f(-0.4f, -0.4f, -0.4f);
	glVertex3f( 0.4f, -0.4f, -0.4f);
	glVertex3f( 0.4f, -0.4f, 0.4f);
	glVertex3f(-0.4f, -0.4f, 0.4f);

	// Pravá stìna

	glNormal3f( 1.0f, 0.0f, 0.0f);// Normála
	glColor3ub(0, 255, 255);
	glVertex3f( 0.4f, -0.4f, -0.4f);
	glVertex3f( 0.4f, 0.4f, -0.4f);
	glVertex3f( 0.4f, 0.4f, 0.4f);
	glVertex3f( 0.4f, -0.4f, 0.4f);

	// Levá stìna

	glNormal3f(-1.0f, 0.0f, 0.0f);// Normála
	glColor3ub(255, 255, 0);
	glVertex3f(-0.4f, -0.4f, -0.4f);
	glVertex3f(-0.4f, -0.4f, 0.4f);
	glVertex3f(-0.4f, 0.4f, 0.4f);
	glVertex3f(-0.4f, 0.4f, -0.4f);
	glEnd();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

#define EPS 1e-9

pair<CVector3, CVector3> getNormQuad(CVector3 vNorm)
{
	pair<CVector3, CVector3> vResult;
	if(fabsf(vNorm.y - 1.0f) < EPS)vResult.first = vecCross(vNorm, CVector3(-1, 0, 0));
	else vResult.first = vecCross(vNorm, CVector3(0, 1, 0));
	vecNormalize(vResult.first);
	vResult.second = vecCross(vNorm, vResult.first);
	vecNormalize(vResult.second);

	return vResult;
}