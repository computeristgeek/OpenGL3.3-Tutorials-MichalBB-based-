#include "mainheader.h"

int iWorking = 0;
int iCur = 0, iShowFPS = 1;

char* strNames[] = 
{
	"Roman Lighter",
	"Blasty Blaster",
	"Volcano"
};

CMSMModel mRomanLighter, mBlastyBlaster, mVolcano;
vector<CRomanLighter> rlLighters;
vector<CBlastyBlaster> bbBlasters;
vector<CVolcano> vVolcanos;

//====================

void CRomanLighter::renderAndUpdate()
{
	glPushMatrix();
	glTranslatef(vPos.x, vPos.y, vPos.z);
	glColor3ub(255, 255, 255);
	if(iPartLeft == 0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(255, 255, 255, (BYTE)(fFade*255.0f));
		fFade -= glAp.sof(1.0f);
	}
	mRomanLighter.renderModel();
	if(iPartLeft == 0)glDisable(GL_BLEND);
	if(vPos.y < 0.0f)vPos.y = 0.0f;
	if(vPos.y != 0.0f)
	{
		vPos += vDir;
		vDir.y -= glAp.sof(0.5f);
	}
	glPopMatrix();
	if(!iWorking)return;
	if(fWait <= 0.0f && iPartLeft > 0)
	{
		CParticle pNewBomb;
		pNewBomb.vPos = vPos;
		pNewBomb.vDir = CVector3(randValue(-1.0f, 1.0f), randValue(25.0f, 45.0f), randValue(-1.0f, 1.0f));
		vecNormalize(pNewBomb.vDir);
		pNewBomb.vDir *= randValue(25.0f, 45.0f);
		FOR(i, 3)pNewBomb.bColor[i] = 64+rand()%192;
		pNewBomb.fFade = 2.0f+randValue(0.0f, 2.0f);
		pBombs.push_back(pNewBomb);
		fWait = 1.0f+randValue(0.0f, 1.2f);
		iPartLeft--;
	}
	fWait -= glAp.sof(1.0f);
	FOR(i, ESZ(pBombs))
	{
		if(pBombs[i].fFade-glAp.sof(1.0f) <= 0.0f)
		{
			BYTE bColor[3]; bColor[0] = bColor[1] = bColor[2] = pBombs[i].bColor[0];
			createParticles(pBombs[i].vPos, 10.0f, 20, bColor);
		}
	}
	renderAllParticles(pBombs, 0);
}

//====================

void CBlastyBlaster::renderAndUpdate()
{
	glPushMatrix();
	glTranslatef(vPos.x, vPos.y, vPos.z);
	glColor3ub(255, 255, 255);
	if(iPartLeft == 0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(255, 255, 255, (BYTE)(fFade*255.0f));
		fFade -= glAp.sof(1.0f);
	}
	mBlastyBlaster.renderModel();
	if(iPartLeft == 0)glDisable(GL_BLEND);
	if(vPos.y < 0.0f)vPos.y = 0.0f;
	if(vPos.y != 0.0f)
	{
		vPos += vDir;
		vDir.y -= glAp.sof(0.5f);
	}
	glPopMatrix();
	if(!iWorking)return;
	if(fWait <= 0.0f && iPartLeft > 0)
	{
		CParticle pNewBomb;
		CVector3 vStart = vPos; vStart.x -=  0.5f; vStart.z -= 0.5f;
		pNewBomb.vPos = vStart; pNewBomb.vPos.x += 0.2f*(iPos%5); pNewBomb.vPos.z += 0.2f*(iPos/5);
		pNewBomb.vDir = CVector3(0, randValue(35.0f, 55.0f), 0);
		FOR(i, 3)pNewBomb.bColor[i] = 0;
		pNewBomb.bColor[rand()%3] = 192+rand()%64;
		pNewBomb.bColor[rand()%3] = 192+rand()%64;
		pNewBomb.fFade = 1.5f+randValue(0.0f, 1.0f);
		pBombs.push_back(pNewBomb);
		fWait = 1.0f+randValue(0.0f, 1.2f);
		iPos++;
		if(iPos == 25){iPos = 0; iPartLeft--; fWait = 1.5f+randValue(0.0f, 1.0f);}
		else fWait = 0.1f;
	}
	fWait -= glAp.sof(1.0f);
	FOR(i, ESZ(pBombs))
	{
		if(pBombs[i].fFade-glAp.sof(1.0f) <= 0.0f)
		{
			BYTE bColor[3]; FOR(j, 3)bColor[j] = pBombs[i].bColor[j];
			createParticles(pBombs[i].vPos, 10.0f, 20, bColor);
		}
	}
	renderAllParticles(pBombs, 0);
}

//====================

void CVolcano::renderAndUpdate()
{
	glPushMatrix();
	glTranslatef(vPos.x, vPos.y, vPos.z);
	glColor3ub(255, 255, 255);
	if(iUp == -1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(255, 255, 255, (BYTE)(fFade*255.0f));
		fFade -= glAp.sof(1.0f);
	}
	mVolcano.renderModel();
	if(iUp == -1)glDisable(GL_BLEND);
	if(vPos.y < 0.0f)vPos.y = 0.0f;
	if(vPos.y != 0.0f)
	{
		vPos += vDir;
		vDir.y -= glAp.sof(0.5f);
	}
	glPopMatrix();
	if(!iWorking)return;
	if(fWait <= 0.0f && iUp >= 0)
	{
		FOR(i, 12)
		{
			CParticle pNewPart;
			pNewPart.vPos = vPos;
			pNewPart.vPos.y += 1.5f;
			pNewPart.vDir = CVector3(randValue(-fHeight/8.0f, fHeight/8.0f), randValue(max(0.0f, fHeight-10.0f), fHeight), randValue(-fHeight/8.0f, fHeight/8.0f));
			pNewPart.bColor[0] = 255-rand()%64;
			pNewPart.bColor[1] = 128-rand()%32;
			pNewPart.bColor[2] = 0;
			pNewPart.fFade = 1.5f+randValue(0.0f, 1.2f);
			pParts.push_back(pNewPart);
		}
		fWait = 0.1f;
		if(iUp == 1){fHeight += 0.15f; if(fHeight > 20.0f)iUp = 0;}
		else if(iUp == 0){fHeight -= 0.15f; if(fHeight < 2.0f)iUp = -1;}
	}
	fWait -= glAp.sof(1.0f);
	renderAllParticles(pParts, 1);
}

//====================

void loadFireworksModels()
{
	mRomanLighter.loadModel("models\\romanlighter.msm");
	mBlastyBlaster.loadModel("models\\blastyblaster.msm");
	mVolcano.loadModel("models\\volcano.msm");
}

//====================

void updateFireworks()
{
	static float fTime = -100.0f;
	if(glAp.onekey(' '))
	{
		if(iWorking)
		{
			rlLighters.clear();
			bbBlasters.clear();
			vVolcanos.clear();
			iWorking = 0;
			fTime = -100.0f;
		}
		else fTime = 3.0f;
	}
	if(fTime > 0.0f && !iWorking)fTime -= glAp.sof(1.0f);
	if(fTime <= 0.0f && fTime >= -10.0f && !iWorking)iWorking = 1;
	if(glAp.onekey(VK_RBUTTON))iCur = (iCur+1)%3;
	if(glAp.onekey('F'))iShowFPS = 1-iShowFPS;
	if(glAp.onekey(VK_LBUTTON))
	{
		if(iCur == 0)
		{
			CRomanLighter rlNew;
			rlNew.iPartLeft = 12;
			rlNew.vDir = camMain.vView-camMain.vEye;
			rlNew.vDir /= 5.0f;
			rlNew.vPos = camMain.vView;
			rlLighters.push_back(rlNew);
		}
		else if(iCur == 1)
		{
			CBlastyBlaster bbNew;
			bbNew.iPartLeft = 4;
			bbNew.vDir = camMain.vView-camMain.vEye;
			bbNew.vDir /= 5.0f;
			bbNew.vPos = camMain.vView;
			bbBlasters.push_back(bbNew);
		}
		else if(iCur == 2)
		{
			CVolcano vNew;
			vNew.vDir = camMain.vView-camMain.vEye;
			vNew.vDir /= 5.0f;
			vNew.vPos = camMain.vView;
			vVolcanos.push_back(vNew);
		}
	}
	FOR(i, ESZ(rlLighters))
	{
		rlLighters[i].renderAndUpdate();
		if(rlLighters[i].fFade <= 0.0f)
		{
			rlLighters.erase(rlLighters.begin()+i);
			i--;
			if(ESZ(rlLighters) == 0 && ESZ(bbBlasters) == 0 && ESZ(vVolcanos) == 0)
			{
				iWorking = 0;
				fTime = -100.0f;
			}
		}
	}
	FOR(i, ESZ(bbBlasters))
	{
		bbBlasters[i].renderAndUpdate();
		if(bbBlasters[i].fFade <= 0.0f)
		{
			bbBlasters.erase(bbBlasters.begin()+i);
			i--;
			if(ESZ(rlLighters) == 0 && ESZ(bbBlasters) == 0 && ESZ(vVolcanos) == 0)
			{
				iWorking = 0;
				fTime = -100.0f;
			}
		}
	}
	FOR(i, ESZ(vVolcanos))
	{
		vVolcanos[i].renderAndUpdate();
		if(vVolcanos[i].fFade <= 0.0f)
		{
			vVolcanos.erase(vVolcanos.begin()+i);
			i--;
			if(ESZ(rlLighters) == 0 && ESZ(bbBlasters) == 0 && ESZ(vVolcanos) == 0)
			{
				iWorking = 0;
				fTime = -100.0f;
			}
		}
	}
	ortho2DBegin(glAp.dm.iScrX, glAp.dm.iScrY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int iXSize = ft.getxsize(FT(strNames[iCur]), 20);
	glColor3ub(255, 0, 0);
	ft.print(CFData(20, glAp.dm.iScrX-10-iXSize, 30), strNames[iCur]);
	if(fTime > 0.0f && !iWorking)
	{
		int iSize = 35+(int)((fTime-(float)((int)fTime))*14.0f);
		char strText[256]; sprintf(strText, "Fireworks begin in %d...", (int)fTime+1);
		int iXsize = ft.getxsize(FT(strText), iSize);
		glColor3ub(0, 0, 255);
		ft.print(CFData(iSize, ft.getcenterx(iXsize, 0, glAp.dm.iScrX), glAp.dm.iScrY/2-iSize/2), strText);
	}
	if(iShowFPS)
	{
		glColor3ub(255, 255, 255);
		ft.print(CFData(16, 10, glAp.dm.iScrY - 20), "FPS: %d", glAp.FPS);
	}
	glDisable(GL_BLEND);
	ortho2DEnd();
}