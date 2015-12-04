#include "mainheader.h"

#define VERSION "MSMV1.20"

//====================

void CMSMModel::loadModel(char* strFile)
{
	FILE* fp = fopen(strFile, "rb");
	if(!fp)return;
	if(strcmp(VERSION, readline(fp))){fclose(fp); return;}
	
	fread(&mHeader, 1, sizeof(MSHeader), fp);
	// Allocate memory
	gGroups = new CGroup[mHeader.nGroups];
	mMats = new CMaterial[mHeader.nMaterials];
	FOR(i, mHeader.nGroups)
	{
		readline(fp); // Ignore group name
		fread(&gGroups[i].cMat, 1, sizeof(char), fp);
		fread(&gGroups[i].nFaces, 1, sizeof(WORD), fp);
		gGroups[i].vVerts = new CVector3[gGroups[i].nFaces*3];
		gGroups[i].vNorms = new CVector3[gGroups[i].nFaces];
		gGroups[i].vCoords = new CVector2[gGroups[i].nFaces*3];
		FOR(j, gGroups[i].nFaces)
		{
			fread(gGroups[i].vVerts+j*3, 3, sizeof(CVector3), fp);
			fread(gGroups[i].vCoords+j*3, 3, sizeof(CVector2), fp);
			gGroups[i].vNorms[j] = vecCross(gGroups[i].vVerts[j*3]-gGroups[i].vVerts[j*3+1],
											gGroups[i].vVerts[j*3+1]-gGroups[i].vVerts[j*3+2]);
			
			vecNormalize(gGroups[i].vNorms[j]);
		}
	}
	int tIndices[150];
	changeDir(fileDir(strFile));
	FOR(i, mHeader.nTextures)
	{
		char* strName = readline(fp);
		int iFound = -1; FOR(j, ESZ(tTextures))if(tTextures[j].first == strName){iFound = j; break;}
		tIndices[i] = iFound == -1 ? ESZ(tTextures) : iFound;
		if(iFound == -1)
		{
			tTextures.push_back(make_pair(strName, 0));
			textureBMP(&tTextures[ESZ(tTextures)-1].second, strName, false, false, LINEAR);
		}
	}
	changeDir("");
	FOR(i, mHeader.nMaterials)
	{
		readline(fp); // Ignore material name
		char cTex; fread(&cTex, sizeof(char), 1, fp);
		mMats[i].iTexture = cTex != -1 ? tIndices[cTex] : -1;
		BYTE bArray[4]; float fArray[4];
		fread(bArray, 4, sizeof(BYTE), fp);
		FOR(j, 4)fArray[j] = (float)bArray[j]/255.0f; FOR(j, 4)mMats[i].fAmbient[j] = fArray[j];
		fread(bArray, 4, sizeof(BYTE), fp);
		FOR(j, 4)fArray[j] = (float)bArray[j]/255.0f; FOR(j, 4)mMats[i].fDiffuse[j] = fArray[j];
		fread(bArray, 4, sizeof(BYTE), fp);
		FOR(j, 4)fArray[j] = (float)bArray[j]/255.0f; FOR(j, 4)mMats[i].fSpecular[j] = fArray[j];
		fread(bArray, 4, sizeof(BYTE), fp);
		FOR(j, 4)fArray[j] = (float)bArray[j]/255.0f; FOR(j, 4)mMats[i].fEmmisive[j] = fArray[j];
		fread(&mMats[i].bShininess, sizeof(BYTE), 1, fp);
	}
	fclose(fp);
}

//====================

void CMSMModel::renderModel()
{
	glEnable(GL_CULL_FACE); glCullFace(GL_BACK);
	FOR(i, mHeader.nGroups)
	{
		if(gGroups[i].cMat != -1)
		{
			if(mMats[gGroups[i].cMat].iTexture != -1)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tTextures[mMats[gGroups[i].cMat].iTexture].second);
			}
			else glDisable(GL_TEXTURE_2D);
			glMaterialfv(GL_FRONT, GL_AMBIENT, mMats[gGroups[i].cMat].fAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mMats[gGroups[i].cMat].fDiffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mMats[gGroups[i].cMat].fSpecular);
			glMaterialfv(GL_FRONT, GL_EMISSION, mMats[gGroups[i].cMat].fEmmisive);
			glMateriali(GL_FRONT, GL_SHININESS, mMats[gGroups[i].cMat].bShininess);
		}
		else glDisable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		FOR(j, gGroups[i].nFaces)
		{
			
			FOR(k, 3)
			{
				glNormal3fv(gGroups[i].vNorms[j].v);
				glTexCoord2fv(gGroups[i].vCoords[j*3+k].v);
				glVertex3fv(gGroups[i].vVerts[j*3+k].v);
			}
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
}