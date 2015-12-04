#include "mainheader.h"

vector<string> strMNames; // Names of materials already loaded
vector<GLuint> uiStor; // GLuint storage for texture
vector<char> cTrans; // If the material is transparent

//====================

GLvoid pri(GLint tt){char ttt[222]; sprintf(ttt, "NUMBWER IS %d", tt); MessageBox(NULL, ttt, "TTT", MB_OK);}
GLvoid CMSModel::loadModelData(char* strFile)
{
	FILE* fp = fopen(strFile, "rb");
	if(fp == NULL)
	{
		char strText[255]; sprintf(strText, "Model %s wasn't found! Re-install application!", strFile);
		MessageBox(glAp.hWindow, strText, "Error", MB_ICONERROR | MB_OK);
		return;
	}
	char strLine[50]; fgets(strLine, 50, fp);
	string str4 = strLine; str4 = str4.substr(0, 4); 
	if(str4 != "MSMV")
	{
		char strMsg[255]; sprintf(strMsg, "The model %s isn't a MS Model File!", strFile);
		MessageBox(glAp.hWindow, strMsg, "Error loading model", MB_ICONERROR | MB_OK);
		fclose(fp);
		return;
	}
	fread(&wNGroups, sizeof(WORD), 1, fp);
	WORD iNMat; fread(&iNMat, sizeof(WORD), 1, fp);
	fread(&wNAnims, sizeof(WORD), 1, fp);

	gGroups = new CGroup[wNGroups];
	FOR(i, wNGroups)
	{
		fgets(strLine, 255, fp); // Name of group, just ignore
		fread(&gGroups[i].cMIndex, sizeof(char), 1, fp);
		fread(&gGroups[i].bBlend, sizeof(BYTE), 1, fp);
		fread(&gGroups[i].wNFaces, sizeof(WORD), 1, fp);
		gGroups[i].vVerts = new CVector3[gGroups[i].wNFaces * 3];
		gGroups[i].vCoords = new CVector2[gGroups[i].wNFaces * 3];
		gGroups[i].vNorms3 = new CVector3[gGroups[i].wNFaces * 3];
		gGroups[i].vNorms = new CVector3[gGroups[i].wNFaces];
		FOR(j, gGroups[i].wNFaces)
		{
			FOR(k, 3)
			{
				fread(&gGroups[i].vVerts[j*3 + k], sizeof(CVector3), 1, fp);
				fread(&gGroups[i].vCoords[j*3 + k], sizeof(CVector2), 1, fp);
			}
			gGroups[i].vNorms3[j*3] = vecCross(gGroups[i].vVerts[j*3] - gGroups[i].vVerts[j*3 + 1], gGroups[i].vVerts[j*3 + 1] - gGroups[i].vVerts[j*3 + 2]);
			vecNormalize(gGroups[i].vNorms3[j*3]);
			gGroups[i].vNorms[j] = gGroups[i].vNorms3[j*3];
			FOR(k, 3)if(k != 0)gGroups[i].vNorms3[j*3 + k] = gGroups[i].vNorms3[j*3];
		}
	}

	//-------------------
	
	int* mInd = new int[iNMat];
		
	FOR(i, iNMat)
	{
		fgets(strLine, 255, fp);
		string strName = strLine; strName = strName.substr(0, strName.length() - 1);
		char trans; fread(&trans, sizeof(char), 1, fp);
		GLboolean bFound = false; GLint iInd = -1;
		FOR(j, ESZ(strMNames))if(strMNames[j] == strName && trans == cTrans[j]){iInd = j; break;}
		if(iInd == -1)
		{
			strMNames.push_back(strName);
			uiStor.push_back(0);
			cTrans.push_back(trans);
			textureBMP(&uiStor[ESZ(uiStor) - 1], (char*)strName.c_str(), false, trans ? true : false, LINEAR);
			mInd[i] = ESZ(uiStor) - 1;
		}
		else mInd[i] = iInd;
	}
	FOR(i, wNGroups)if(gGroups[i].cMIndex != -1)gGroups[i].cMIndex = mInd[gGroups[i].cMIndex];

	//-------------------

	aAnims = new CAnimation[wNAnims];
	FOR(i, wNAnims)
	{
		fgets(strLine, 255, fp); aAnims[i].strName = strLine;
		aAnims[i].strName = aAnims[i].strName.substr(0, aAnims[i].strName.length() - 1);
		fread(&aAnims[i].bData, sizeof(BYTE), 1, fp);
		fread(&aAnims[i].bNumKF, sizeof(BYTE), 1, fp);
		aAnims[i].vVerts = new CVector3**[aAnims[i].bNumKF];
		aAnims[i].vCoords = new CVector2**[aAnims[i].bNumKF];
		aAnims[i].vNorms = new CVector3**[aAnims[i].bNumKF];
		aAnims[i].bKFData = new BYTE[aAnims[i].bNumKF];
		aAnims[i].bBitFld = new BYTE*[aAnims[i].bNumKF];
		aAnims[i].fTime = new float[aAnims[i].bNumKF];
		aAnims[i].fBlend = new float*[aAnims[i].bNumKF];
		aAnims[i].cTexture = new char*[aAnims[i].bNumKF];

		FOR(j, aAnims[i].bNumKF)
		{
			aAnims[i].vVerts[j] = new CVector3*[wNGroups];
			aAnims[i].vCoords[j] = new CVector2*[wNGroups];
			aAnims[i].vNorms[j] = new CVector3*[wNGroups];
			aAnims[i].cTexture[j] = new char[wNGroups];
			aAnims[i].fBlend[j] = new float[wNGroups];
			if(j == 0) // Initiate first
			FOR(k, wNGroups)
			{
				aAnims[i].cTexture[j][k] = gGroups[k].cMIndex;
				aAnims[i].fBlend[j][k] = (float)gGroups[k].bBlend / 255.0f;
			}
			aAnims[i].bBitFld[j] = new BYTE(1 + wNGroups / 8);

			DWORD dwTime; fread(&dwTime, sizeof(DWORD), 1, fp);
			aAnims[i].fTime[j] = (float)dwTime;
			fread(&aAnims[i].bKFData[j], sizeof(BYTE), 1, fp);

			FOR(k, 1 + wNGroups / 8)fread(&aAnims[i].bBitFld[j][k], sizeof(BYTE), 1, fp);
			
			FOR(k, wNGroups)
			{
				if(aAnims[i].bBitFld[j][k / 8] & (1 << (k % 8)))
				{
					char cTexture; fread(&cTexture, sizeof(char), 1, fp);
					if(cTexture != -1)aAnims[i].cTexture[j][k] = mInd[cTexture];
					else aAnims[i].cTexture[j][k] = -1;
					BYTE bBlend; fread(&bBlend, sizeof(BYTE), 1, fp);
					aAnims[i].fBlend[j][k] = (float)bBlend / 255.0f;
					if(aAnims[i].bKFData[j] & 2) // If animating vertices
					{
						aAnims[i].vVerts[j][k] = new CVector3[gGroups[k].wNFaces * 3];
						aAnims[i].vNorms[j][k] = new CVector3[gGroups[k].wNFaces];
						FOR(l, gGroups[k].wNFaces)
						{
							FOR(m, 3)fread(&aAnims[i].vVerts[j][k][l*3 + m], 1, sizeof(CVector3), fp);
							aAnims[i].vNorms[j][k][l] = vecCross(aAnims[i].vVerts[j][k][l*3] - aAnims[i].vVerts[j][k][l*3 + 1], aAnims[i].vVerts[j][k][l*3 + 1] - aAnims[i].vVerts[j][k][l*3 + 2]);
							vecNormalize(aAnims[i].vNorms[j][k][l]);
						}
					}

					if(aAnims[i].bKFData[j] & 4) // If animating coords
					{
						aAnims[i].vCoords[j][k] = new CVector2[gGroups[k].wNFaces * 3];
						FOR(l, gGroups[k].wNFaces)FOR(m, 3)fread(&aAnims[i].vCoords[j][k][l*3 + m], 1, sizeof(CVector2), fp);
					}
				}
				else if(j != 0)
				{
					aAnims[i].cTexture[j][k] = aAnims[i].cTexture[j - 1][k];
					aAnims[i].fBlend[j][k] = aAnims[i].fBlend[j - 1][k];
				}
			}
		}
	}
	fclose(fp);

	//-------------------
	
	FOR(i, wNAnims)
	{
		aAnims[i].ptrVert1 = new CVector3**[aAnims[i].bNumKF];
		aAnims[i].ptrVert2 = new CVector3**[aAnims[i].bNumKF];
		aAnims[i].ptrNorm1 = new CVector3**[aAnims[i].bNumKF];
		aAnims[i].ptrNorm2 = new CVector3**[aAnims[i].bNumKF];
		aAnims[i].ptrCoord1 = new CVector2**[aAnims[i].bNumKF];
		aAnims[i].ptrCoord2 = new CVector2**[aAnims[i].bNumKF];

		FOR(j, aAnims[i].bNumKF)
		{
			aAnims[i].ptrVert1[j] = new CVector3*[wNGroups];
			aAnims[i].ptrVert2[j] = new CVector3*[wNGroups];
			aAnims[i].ptrNorm1[j] = new CVector3*[wNGroups];
			aAnims[i].ptrNorm2[j] = new CVector3*[wNGroups];
			aAnims[i].ptrCoord1[j] = new CVector2*[wNGroups];
			aAnims[i].ptrCoord2[j] = new CVector2*[wNGroups];
		}

		FOR(j, wNGroups) // Set first keyframe as base for dynamic programming
		{
			GLint iInd = j / 8, iBit = j % 8;
			if(aAnims[i].bBitFld[0][iInd] & (1 << iBit))
			{
				if(aAnims[i].bKFData[0] & 2) // If animating vertices
				{
					aAnims[i].ptrVert1[0][j] = aAnims[i].vVerts[0][j];
					aAnims[i].ptrNorm1[0][j] = aAnims[i].vNorms[0][j];
				}
				else
				{
					aAnims[i].ptrVert1[0][j] = gGroups[j].vVerts;
					aAnims[i].ptrNorm1[0][j] = gGroups[j].vNorms;
				}

				if(aAnims[i].bKFData[0] & 4) // If animating coordinates
					aAnims[i].ptrCoord1[0][j] = aAnims[i].vCoords[0][j];
				else aAnims[i].ptrCoord1[0][j] = gGroups[j].vCoords;
			}
			else
			{
				aAnims[i].ptrVert1[0][j] = gGroups[j].vVerts;
				aAnims[i].ptrCoord1[0][j] = gGroups[j].vCoords;
				aAnims[i].ptrNorm1[0][j] = gGroups[j].vNorms;
			}
		}

		FOR(j, aAnims[i].bNumKF)
		{
			FOR(k, wNGroups)
			{
				GLint iInd = k / 8, iBit = k % 8, iKF2 = (j + 1) % aAnims[i].bNumKF;
				
				// Here we initiate first pointer to all animation data
				if(j != 0) // Keyframe 1 is already set
				{
					if(aAnims[i].bBitFld[j][iInd] & (1 << iBit))
					{
						if(aAnims[i].bKFData[j] & 2)
						{
							aAnims[i].ptrVert1[j][k] = aAnims[i].vVerts[j][k];
							aAnims[i].ptrNorm1[j][k] = aAnims[i].vNorms[j][k];
						}
						else
						{
							aAnims[i].ptrVert1[j][k] = aAnims[i].ptrVert1[j - 1][k];
							aAnims[i].ptrNorm1[j][k] = aAnims[i].ptrNorm1[j - 1][k];
						}

						if(aAnims[i].bKFData[j] & 4)aAnims[i].ptrCoord1[j][k] = aAnims[i].vCoords[j][k];
						else aAnims[i].ptrCoord1[j][k] = aAnims[i].ptrCoord1[j - 1][k];
					}
					else
					{
						aAnims[i].ptrVert1[j][k] = aAnims[i].ptrVert1[j - 1][k];
						aAnims[i].ptrCoord1[j][k] = aAnims[i].ptrCoord1[j - 1][k];
						aAnims[i].ptrNorm1[j][k] = aAnims[i].ptrNorm1[j - 1][k];
					}
				}
				// Here we initiate second pointer to all animation data
				if(aAnims[i].bBitFld[iKF2][iInd] & (1 << iBit))
				{
					if(aAnims[i].bKFData[iKF2] & 2)
					{
						aAnims[i].ptrVert2[j][k] = aAnims[i].vVerts[iKF2][k];
						aAnims[i].ptrNorm2[j][k] = aAnims[i].vNorms[iKF2][k];
					}
					else
					{
						aAnims[i].ptrVert2[j][k] = NULL;
						aAnims[i].ptrNorm2[j][k] = NULL;
					}

					if(aAnims[i].bKFData[iKF2] & 4)
						aAnims[i].ptrCoord2[j][k] = aAnims[i].vCoords[iKF2][k];
					else aAnims[i].ptrCoord2[j][k] = NULL;
				}
				else if(iKF2 == 0)
				{
					aAnims[i].ptrVert2[j][k] = aAnims[i].ptrVert1[0][k];
					aAnims[i].ptrNorm2[j][k] = aAnims[i].ptrNorm1[0][k];
					aAnims[i].ptrCoord2[j][k] = aAnims[i].ptrCoord1[0][k];
				}
				else
				{
					aAnims[i].ptrVert2[j][k] = NULL;
					aAnims[i].ptrNorm2[j][k] = NULL;
					aAnims[i].ptrCoord2[j][k] = NULL;
				}
			}
		}
	}
	bLoaded = true;
}

//====================

GLvoid CMSModel::renderModel(CAnimData* aData)
{
	if(!bLoaded)return; // If there are no loaded data, return

	GLint iOnly = 0;
	iOnly |= aData == NULL;
	if(!iOnly)iOnly |= aData->iAnim == -1;
	if(iOnly) // Just render model base using vetex arrays
	{
		FOR(i, wNGroups)
		{
			if(gGroups[i].bBlend == 0)continue;
			glTexCoordPointer(2, GL_FLOAT, 0, gGroups[i].vCoords);
			glVertexPointer(3, GL_FLOAT, 0, gGroups[i].vVerts);
			glNormalPointer(GL_FLOAT, 0, gGroups[i].vNorms3);
			// Bind texture
			if(gGroups[i].cMIndex != -1)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, uiStor[gGroups[i].cMIndex]);
			}
			else glDisable(GL_TEXTURE_2D);
			// Set blending
			BYTE bFinalBlend = (BYTE)((float)gGroups[i].bBlend * fgBlend);
			if(bFinalBlend < 255){glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);}
			else glDisable(GL_BLEND);
			glColor4ub(255, 255, 255, bFinalBlend);
			// Render arrays
			glDrawArrays(GL_TRIANGLES, 0, gGroups[i].wNFaces * 3);
		}
	}
	else
	{
		GLint iKF = aData->iKF; // Actual keyframe
		GLint iAnim = aData->iAnim;
		GLint iKF2 = (iKF + 1) % aAnims[iAnim].bNumKF; // Next keyframe
		
		if(!aData->bPaused)aData->fSTime = (float)GetTickCount();
		GLfloat fScale = (aData->fSTime - aData->fBTime) / aAnims[iAnim].fTime[iKF2];
		FOR(i, wNGroups)
		{
			GLfloat fBlend = aAnims[iAnim].fBlend[iKF][i];
			GLfloat fDif = aAnims[iAnim].fBlend[iKF2][i] - aAnims[iAnim].fBlend[iKF][i];
			if(!(aAnims[iAnim].bKFData[iKF2] & 1))fBlend += fDif * fScale;
			fBlend *= fgBlend;
			if(fBlend <= 1.0f){glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);}
			if(fBlend == 0.0f)continue;
			glColor4f(1.0f, 1.0f, 1.0f, fBlend); // Set blending later
			if(aAnims[iAnim].cTexture[iKF][i] != -1){glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, uiStor[aAnims[iAnim].cTexture[iKF][i]]);}
			else glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
			CVector3* vVerts1 = aAnims[iAnim].ptrVert1[iKF][i], *vVerts2 = aAnims[iAnim].ptrVert2[iKF][i];
			CVector3* vNorms1 = aAnims[iAnim].ptrNorm1[iKF][i], *vNorms2 = aAnims[iAnim].ptrNorm2[iKF][i];
			CVector2* vCoords1 = aAnims[iAnim].ptrCoord1[iKF][i], *vCoords2 = aAnims[iAnim].ptrCoord2[iKF][i];
			FOR(j, gGroups[i].wNFaces)
			{
				CVector3 vNorm = vNorms1[j];
				if(vNorms2 != NULL && !(aAnims[iAnim].bKFData[iKF2] & 1))vNorm += (vNorms2[j] - vNorms1[j]) * fScale;
				glNormal3f(vNorm.x, vNorm.y, vNorm.z);
				FOR(k, 3)
				{
					CVector3 vPoGLint = vVerts1[j*3 + k];
					CVector2 vCoord = vCoords1[j*3 + k];
					if(vVerts2 != NULL && !(aAnims[iAnim].bKFData[iKF2] & 1))vPoGLint += (vVerts2[j*3 + k] - vVerts1[j*3 + k]) * fScale;
					if(vCoords2 != NULL && !(aAnims[iAnim].bKFData[iKF2] & 1))vCoord += (vCoords2[j*3 + k] - vCoords1[j*3 + k]) * fScale;
					
					glTexCoord2f(vCoord.x, vCoord.y);
					glVertex3f(vPoint.x, vPoint.y, vPoint.z);
				}
			}
			glEnd();
			if(fBlend <= 1.0f)glDisable(GL_BLEND);
		}
		if(!aData->bPaused)
		{
			GLfloat fDiff = aData->fSTime - aData->fBTime;
			if(fDiff > aAnims[iAnim].fTime[iKF2])
			{
				aData->fBTime = aData->fSTime - (fDiff - aAnims[iAnim].fTime[iKF2]);
				aData->iKF = (iKF + 1) % aAnims[iAnim].bNumKF;
				GLint iLast = aAnims[iAnim].bData & 1 ? aAnims[iAnim].bNumKF - 1 : 0;
				if(aData->iKF == iLast)
				{
					FOR(i, aAnims[iAnim].bNumKF)aData->bCanRetKFP[i] = true;
					aData->iLoops++;
					if(aData->iLoops == aData->iLoop) // The end of animation
					{
						if(aData->bPauseAtEnd)aData->pauseAnim();
						else aData->iAnim = -1;
					}
					else aData->iKF = 0;
				}
			}
		}
	}
}

//====================

GLvoid CMSModel::startAnimation(char* strName, GLint iLoop, CAnimData *aData)
{
	GLint iInd = -1;
	FOR(i, wNAnims)if(strName == aAnims[i].strName){iInd = i; break;}
	if(iInd != -1)
	{
		aData->iAnim = iInd;
		aData->iKF = 0;
		aData->iLoop = iLoop;
		aData->iLoops = 0;
		aData->fBTime = (float)GetTickCount();
		aData->bCanRetKFP = new bool[aAnims[iInd].bNumKF];
		FOR(i, aAnims[iInd].bNumKF)aData->bCanRetKFP[i] = true;
		aData->bPaused = false;
		aData->bPauseAtEnd = false;
	}
}

//====================

GLvoid CMSModel::setGlobalBlend(GLfloat sfgBlend){fgBlend = sfgBlend;}

//====================

GLboolean CAnimData::kfPassed(GLint iKFrame)
{
	if(bCanRetKFP[iKFrame]){bCanRetKFP[iKFrame] = false; return true;}
	return false;
}

//====================

GLvoid CAnimData::pauseAnim()
{
	if(bPaused)return;
	bPaused = true;
	fSTime = (float)GetTickCount();
}

//====================

GLvoid CAnimData::continueAnim()
{
	if(!bPaused)return;
	bPaused = false;
	fBTime += (float)GetTickCount() - fSTime;
}