#include "mainheader.h"

char* strNames[] = 
{
	"tower_base.msm",
	"tower_cannon.msm",
	"spider.msm",
	"spider_nest.msm",
	"spider_ball.msm",
	"spider_frost.msm",
	"spider_nest_frost.msm",
	"spider_ball_frost.msm",
};

CMSModel* mEnemies;

vector<CTower> tTowers;
vector<CSpider> sSpiders;

//====================

void loadEnemies(int iNModels)
{
	char strCur[512]; GetCurrentDirectory(512, strCur);
	char strOld[512]; strcpy(strOld, strCur);
	strcat(strCur, "\\Enemies");
	SetCurrentDirectory(strCur);
	mEnemies = new CMSModel[iNModels];
	FOR(i, iNModels)mEnemies[i].loadModelData(strNames[i]);
	FILE* fp = fopen("Enemies.txt", "rt");
	char strLine[255]; int iNum;
	sscanf(readline(fp, strLine), "TOWERS %d", &iNum);
	CTower tNew;
	sscanf(readline(fp, strLine), "COLBOX %d", &tNew.iNumCB);
	tNew.iNumPoints = new int[tNew.iNumCB];
	tNew.vColBox = new CVector3*[tNew.iNumCB];
	tNew.fLife = 100.0f;
	FOR(i, tNew.iNumCB)
	{
		sscanf(readline(fp, strLine), "POINTS %d", &tNew.iNumPoints[i]);
		tNew.vColBox[i] = new CVector3[tNew.iNumPoints[i]];
		FOR(j, tNew.iNumPoints[i])sscanf(readline(fp, strLine), "%f %f %f", &tNew.vColBox[i][j].x, &tNew.vColBox[i][j].y, &tNew.vColBox[i][j].z);
	}
	FOR(i, iNum)
	{
		sscanf(readline(fp, strLine), "%f %f %f", &tNew.vPos.x, &tNew.vPos.y, &tNew.vPos.z);
		tTowers.push_back(tNew);
		tTowers[i].iNumPoints = new int[tNew.iNumCB];
		tTowers[i].vColBox = new CVector3*[tNew.iNumCB];
		FOR(j, tNew.iNumCB)
		{
			tTowers[i].iNumPoints[j] = tNew.iNumPoints[j];
			tTowers[i].vColBox[j] = new CVector3[tNew.iNumPoints[j]];
			FOR(k, tNew.iNumPoints[j])tTowers[i].vColBox[j][k] = tNew.vPos + tNew.vColBox[j][k];
		}
	}
	sscanf(readline(fp, strLine), "SPIDERWEB %d", &iNum);
	CSpiderWeb swNew;
	swNew.fTime = 0.0f;
	FOR(i, iNum)
	{
		FOR(j, 4)sscanf(readline(fp, strLine), "%f %f %f", &swNew.vPoints[j].x, &swNew.vPoints[j].y, &swNew.vPoints[j].z);
		swNew.iAll = 0;
		char strType[20] = {0};
		sscanf(readline(fp, strLine), "TYPE %s", strType);
		swNew.iType = strcmp(strType, "NORMAL");
		sscanf(readline(fp, strLine), "NESTS %d", &swNew.iNests);
		swNew.iNSpiders = new int[swNew.iNests];
		swNew.vNests = new CVector3[swNew.iNests];
		swNew.iNAll = new int[swNew.iNests];
		CVector3 vDirA = swNew.vPoints[1] - swNew.vPoints[0], vDirB = swNew.vPoints[2] - swNew.vPoints[1];
		float fLenA = vecMagnitude(vDirA), fLenB = vecMagnitude(vDirB);
		vecNormalize(vDirA); vecNormalize(vDirB);
		FOR(j, swNew.iNests)
		{
			sscanf(readline(fp, strLine), "%d", &swNew.iNSpiders[j]);
			swNew.iNAll[j] = swNew.iNSpiders[j];
			swNew.iAll += swNew.iNSpiders[j];
			float fX = fLenA * (float)(rand() % 100) * 0.01f;
			float fY = fLenB * (float)(rand() % 100) * 0.01f;
			swNew.vNests[j] = swNew.vPoints[0] + vDirA * fX + vDirB * fY;
		}
		swWebs.push_back(swNew);
	}
	fclose(fp);
	SetCurrentDirectory(strOld);
}

//====================

void renderEnemies()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	FOR(i, ESZ(tTowers))
	{
		glPushMatrix();
		glTranslatef(tTowers[i].vPos.x, tTowers[i].vPos.y, tTowers[i].vPos.z);
		tTowers[i].render();
		glPopMatrix();
		if(tTowers[i].fFade < 0.0f)
		{
			tTowers.erase(tTowers.begin() + i);
			i--;
		}
	}
	FOR(i, ESZ(sSpiders))
	{
		glPushMatrix();
		glTranslatef(sSpiders[i].vPos.x, sSpiders[i].vPos.y, sSpiders[i].vPos.z);
		sSpiders[i].render();
		glPopMatrix();
		FOR(j, ESZ(sSpiders))
		{
			if(i != j && !sSpiders[i].bDying && !sSpiders[j].bDying)
			{
				float fDist = vecDist(sSpiders[i].vPos, sSpiders[j].vPos);
				if(fDist < 5.0f)
				{
					CVector3 vDir = sSpiders[i].vLastPos - sSpiders[i].vPos;
					if(vecMagnitude(vDir) != 0.0f)
					{
						vecNormalize(vDir);
						float fMove = 5.05f - fDist;
						sSpiders[i].vPos += vDir * fMove;
					}
				}
			}
		}
		if(sSpiders[i].fFade < 0.0f)
		{
			sSpiders.erase(sSpiders.begin() + i);
			i--;
		}
	}
	glDisable(GL_TEXTURE_2D);
	FOR(i, ESZ(sSpiders))sSpiders[i].renderBlood();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
}

//====================

void renderBlendedEnemyStuff()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, uiSmokeTex);
	FOR(i, ESZ(tTowers))tTowers[i].renderSmoke();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	FOR(i, ESZ(swWebs))swWebs[i].render();
	glDisable(GL_BLEND);
}

//====================

int collisionLineEnemies(CVector3 vA, CVector3 vB)
{
	FOR(i, ESZ(tTowers))
	FOR(j, tTowers[i].iNumCB)
	if(collisionLinePoly(vA, vB, NULL, tTowers[i].vColBox[j], tTowers[i].iNumPoints[j]))
	{
		iLIE = i; iEnType = 0;
		return 1;
	}
	FOR(i, ESZ(sSpiders))
	{
		CVector3 vPos = sSpiders[i].vPos; if(!sSpiders[i].bDying)vPos.y += 1.0f;
		float fRadius = sSpiders[i].iType == 2 ? 1.5f : 2.5f;
		if(collisionSphereLine(vPos, fRadius, vA, vB) && sSpiders[i].fLife >= -25.0f)
		{
			iLIE = i; iEnType = 1;
			return 1;
		}
	}
	return 0;
}

//====================

void takeEnemiesLife(float fDamage)
{
	if(iEnType == 0)tTowers[iLIE].fLife -= fDamage;
	else if(iEnType == 1)
	{
		sSpiders[iLIE].fLife -= fDamage;
		sSpiders[iLIE].addBlood(sSpiders[iLIE].vPos, fDamage);
	}
}