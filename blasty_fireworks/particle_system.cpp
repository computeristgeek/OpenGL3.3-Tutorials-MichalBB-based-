#include "mainheader.h"

vector<pair<string, UINT> > tTextures;
vector<CParticle> pParts;

//====================

pair<CVector3, CVector3> getNormQuad(CVector3 vNorm)
{
	pair<CVector3, CVector3> vResult;
	if(fabsf(vNorm.y - 1.0f) < 1e-9)vResult.first = vecCross(vNorm, CVector3(-1, 0, 0));
	else vResult.first = vecCross(vNorm, CVector3(0, 1, 0));
	vecNormalize(vResult.first);
	vResult.second = vecCross(vNorm, vResult.first);
	vecNormalize(vResult.second);

	return vResult;
}

//====================

GLvoid renderAllParticles(vector<CParticle> &pParts, GLint iFade)
{
	pair<CVector3, CVector3> vDirs = getNormQuad(camMain.vView-camMain.vEye);
	vDirs.first /= 4.0f; vDirs.second /= 4.0f;
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, tTextures[0].second);
	glBegin(GL_QUADS);
	FOR(i, ESZ(pParts))
	{
		glColor4ub(pParts[i].bColor[0], pParts[i].bColor[1], pParts[i].bColor[2], iFade ? (BYTE)(min(1.0f, pParts[i].fFade)*255) : 255);
		CVector3 vVertex = pParts[i].vPos-vDirs.first+vDirs.second;
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(vVertex.v);
		vVertex = pParts[i].vPos+vDirs.first+vDirs.second;
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(vVertex.v);
		vVertex = pParts[i].vPos+vDirs.first-vDirs.second;
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(vVertex.v);
		vVertex = pParts[i].vPos-vDirs.first-vDirs.second;
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(vVertex.v);
		pParts[i].vPos += pParts[i].vDir*glAp.sof(1.0f);
		pParts[i].vDir.y -= glAp.sof(10.0f);
		pParts[i].fFade -= glAp.sof(1.0f);
		if(pParts[i].fFade <= 0.0f)
		{
			pParts.erase(pParts.begin()+i);
			i--;
		}
	}
	glEnd();
	glDisable(GL_BLEND);
}

//====================

GLfloat randValue(GLfloat fMin, GLfloat fMax)
{
	return fMin+((float)(rand())/(float)RAND_MAX)*(fMax-fMin);
}

//====================

GLvoid createParticles(CVector3 vPos, GLfloat fMaxSpeed, GLint iCount, BYTE* bColor)
{
	FOR(i, iCount)
	{
		CParticle pNew;
		FOR(j, 3)pNew.bColor[j] = bColor == NULL ? rand()%256 : bColor[j];
		pNew.vPos = vPos;
		FOR(j, 3)pNew.vDir.v[j] = randValue(-fMaxSpeed, fMaxSpeed);
		pNew.fFade = 2.0f+randValue(0.0f, 3.0f);
		pParts.push_back(pNew);
	}
}

//====================

GLvoid loadTextures()
{
	tTextures.resize(1);
	textureBMP(&tTextures[0].second, "textures/particle.bmp", false, false, LINEAR);
	tTextures[0].first = "particle.bmp";
}