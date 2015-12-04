#include "mainheader.h"

//====================

void CWorld::loadWorld(char* strFile)
{
	FILE* fp = fopen(strFile, "rt");
	int iCnt, iCnt2;
	sscanf(readline(fp), "TEXTURES %d", &iCnt);
	FOR(i, iCnt)
	{
		char* strTex = readline(fp);
		tTextures.push_back(make_pair(fileName(strTex), 0));
		textureBMP(&tTextures[ESZ(tTextures)-1].second, strTex, false, false, LINEAR);
		uiTextures.push_back(tTextures[ESZ(tTextures)-1].second);
		sscanf(readline(fp), "FACES %d", &iCnt2);
		iNumFaces.push_back(iCnt2);
		FOR(j, iCnt2)
		{
			FOR(k, 3)
			{
				CVector3 vVert; CVector2 vCoord;
				sscanf(readline(fp), "%f %f %f %f %f", &vVert.x, &vVert.y, &vVert.z, &vCoord.x, &vCoord.y);
				vVerts.push_back(vVert); vCoords.push_back(vCoord);
			}
			k = ESZ(vVerts);
			CVector3 vNorm = vecCross(vVerts[k-3]-vVerts[k-2], vVerts[k-2]-vVerts[k-1]);
			vecNormalize(vNorm);
			vNorms.push_back(vNorm);
		}
	}
	sscanf(readline(fp), "OBJECTS %d", &iCnt);
	mModels = new CMSMModel[iCnt];
	FOR(i, iCnt)
	{
		mModels[i].loadModel(readline(fp));
		CObject oNew; oNew.iModel = i;
		sscanf(readline(fp), "BOUNDINGBOXFACES %d", &iCnt2);
		FOR(j, iCnt2)FOR(k, 4)
		{
			CVector3 vVert;
			sscanf(readline(fp), "%f %f %f", &vVert.x, &vVert.y, &vVert.z);
			oNew.vBoundBox.push_back(vVert);
		}
		sscanf(readline(fp), "ALL %d", &iCnt2);
		FOR(j, iCnt2)
		{
			CObject oNew2 = oNew;
			sscanf(readline(fp), "%f %f %f", &oNew2.vPos.x, &oNew2.vPos.y, &oNew2.vPos.z);
			FOR(j, ESZ(oNew2.vBoundBox))
			{
				if(j%4 == 0)
				{
					CVector3 vNorm = vecCross(oNew2.vBoundBox[j]-oNew2.vBoundBox[j+1], oNew2.vBoundBox[j+1]-oNew2.vBoundBox[j+2]);
					vecNormalize(vNorm);
					oNew2.vNorms.push_back(vNorm);
				}
				oNew2.vBoundBox[j] += oNew2.vPos;
			}
			oObjects.push_back(oNew2);
		}
	}
	fclose(fp);
}

//====================

void CWorld::renderWorld()
{
	int l = 0;
	glColor3ub(255, 255, 255);
	glEnable(GL_TEXTURE_2D);
	FOR(i, ESZ(uiTextures))
	{
		glBindTexture(GL_TEXTURE_2D, uiTextures[i]);
		glBegin(GL_TRIANGLES);
		FOR(j, iNumFaces[i])FOR(k, 3)
		{
			glNormal3fv(vNorms[l/3+j].v);
			glTexCoord2fv(vCoords[l+j*3+k].v);
			glVertex3fv(vVerts[l+j*3+k].v);
		}
		l += iNumFaces[i]*3;
		glEnd();
	}
	FOR(i, ESZ(oObjects))
	{
		glPushMatrix();
		glTranslatef(oObjects[i].vPos.x, oObjects[i].vPos.y, oObjects[i].vPos.z);
		mModels[oObjects[i].iModel].renderModel();
		glPopMatrix();
	}
}