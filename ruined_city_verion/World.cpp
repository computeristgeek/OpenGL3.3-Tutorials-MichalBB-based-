#include "mainheader.h"

//====================

char* readline(FILE* fp, char* buffer)
{
	do
	{
		fgets(buffer, 255, fp);
	}
	while(buffer[0] == '\n' || buffer[0] == '/');
	return buffer;
}

//====================

GLvoid CWorld::loadWorld(char* strFile)
{
	FILE* fp = fopen(strFile, "rt");
	char line[255];
	sscanf(readline(fp, line), "TEXTURES %d", &iNumT);
	
	uiTxt = new GLuint[iNumT];
	iFacesT = new int[iNumT];
	vector<CVector3> vPts;
	vector<CVector2> vCrds;
	iAll = 0;
	FOR(i, iNumT)
	{
		string txt = readline(fp, line);
		txt.erase(txt.end() - 1);
		txt = "Textures\\" + txt;
		textureBMP(&uiTxt[i], (char*)txt.c_str(), false, false, MIPMAP);
		sscanf(readline(fp, line), "FACES %d", &iFacesT[i]);
		iAll += iFacesT[i];
		FOR(j, iFacesT[i])
		{
			CVector3 vPt; CVector2 vCrd;
			FOR(k, 3)
			{
				sscanf(readline(fp, line), "%f %f %f %f %f", &vCrd.x, &vCrd.y, &vPt.x, &vPt.y, &vPt.z);
				vPts.push_back(vPt);
				vCrds.push_back(vCrd);
			}
		}
	}
	vFaces = new CVector3[iAll*3];
	vNorms = new CVector3[iAll];
	vCoords = new CVector2[iAll*3];
	fAngles = new float[iAll];
	GLint iCur = 0;
	FOR(i, iNumT)
	{
		for(GLint j = iCur; j < iCur + iFacesT[i]; j++)
		{
			FOR(k, 3)
			{
				vFaces[j*3 + k] = vPts[j*3 + k];
				vCoords[j*3 + k] = vCrds[j*3 + k];
			}
			vNorms[j] = vecCross(vPts[j*3] - vPts[j*3 + 1], vPts[j*3 + 1] - vPts[j*3 + 2]);
			vecNormalize(vNorms[j]);
			GLfloat fAn = (float)vecAngle(vNorms[j], CVector3(0, 1, 0));
			GLfloat fAn2 = (float)vecAngle(vNorms[j] * -1, CVector3(0, 1, 0));
			if(fAn < 0.25f * PI || fAn2 < 0.25f * PI)iStand.push_back(j);
			else iNotStand.push_back(j);
			fAngles[j] = fAn;
		}
		iCur += iFacesT[i];
	}
	sscanf(readline(fp, line), "FLAGS %d", &iNumF);
	vFlags = new CVector3[iNumF];
	fFacc = new float[iNumF];
	fFtime = new float[iNumF];
	fFangle = new float[iNumF];
	FOR(i, iNumF)
	{
		fFacc[i] = 90 + (float)(rand() % 91);
		fFtime[i] = 2 + (float)(rand() % 101) / 10.0f;
		fFangle[i] = (float)(rand() % 360);
		sscanf(readline(fp, line), "%f %f %f", &vFlags[i].x, &vFlags[i].y, &vFlags[i].z);
	}
	fclose(fp);
}

//====================

GLuint uiFlagTex;

//====================

GLvoid CWorld::renderWorld()
{
	glEnable(GL_TEXTURE_2D);
	glColor3ub(255, 255, 255);
	GLint iCur = 0;
	FOR(i, iNumT)
	{
		glBindTexture(GL_TEXTURE_2D, uiTxt[i]);
		glBegin(GL_TRIANGLES);
		for(GLint j = iCur; j < iCur + iFacesT[i]; j++)
		{
			glNormal3f(vNorms[j].x, vNorms[j].y, vNorms[j].z);
			FOR(k, 3)
			{
				glTexCoord2f(vCoords[j*3 + k].x, vCoords[j*3 + k].y);
				glVertex3f(vFaces[j*3 + k].x, vFaces[j*3 + k].y, vFaces[j*3 + k].z);
			}
		}
		glEnd();
		iCur += iFacesT[i];
	}
	glDisable(GL_TEXTURE_2D);

	#define FS 0.1f
	#define EACH 30.0f
	#define SZ 0.4f
	#define SZX 0.25f
	FOR(i, iNumF)
	{
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor3ub(128, 64, 0);
			glVertex3f(vFlags[i].x - FS, vFlags[i].y, vFlags[i].z);
			glVertex3f(vFlags[i].x - FS, vFlags[i].y + 8, vFlags[i].z);
			glVertex3f(vFlags[i].x + FS, vFlags[i].y + 8, vFlags[i].z);
			glVertex3f(vFlags[i].x + FS, vFlags[i].y, vFlags[i].z);
		glEnd();
		glColor3ub(255, 255, 255);
		GLfloat fEnd = fFangle[i] + 360.0f;
		GLfloat fCur = fFangle[i];
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, uiFlagTex);
		glBegin(GL_QUADS);
		GLfloat fX = vFlags[i].x + FS;
		GLfloat fX2a = 1.0f / ((360.0f / EACH) + 1);
		GLfloat sin1 = sin(fCur * (PI / 180));
		glTexCoord2f(0, 1); glVertex3f(fX, vFlags[i].y + 8, vFlags[i].z);
		glTexCoord2f(fX2a, 1); glVertex3f(fX + SZX, vFlags[i].y + 8, vFlags[i].z - sin1 * SZ);
		glTexCoord2f(fX2a, 0); glVertex3f(fX + SZX, vFlags[i].y + 6, vFlags[i].z - sin1 * SZ);
		glTexCoord2f(0, 0); glVertex3f(fX, vFlags[i].y + 6, vFlags[i].z);
		GLfloat fX2 = fX2a;
		fX += SZX;
		while(fCur < fEnd)
		{
			sin1 = sin(fCur * (PI / 180));
			fCur += EACH;
			GLfloat sin2 = sin(fCur * (PI / 180));
			glTexCoord2f(fX2, 1); glVertex3f(fX, vFlags[i].y + 8, vFlags[i].z - sin1 * SZ);
			glTexCoord2f(fX2 + fX2a, 1); glVertex3f(fX + SZX, vFlags[i].y + 8, vFlags[i].z - sin2 * SZ);
			glTexCoord2f(fX2 + fX2a, 0); glVertex3f(fX + SZX, vFlags[i].y + 6, vFlags[i].z - sin2 * SZ);
			glTexCoord2f(fX2, 0); glVertex3f(fX, vFlags[i].y + 6, vFlags[i].z - sin1 * SZ);
			fX += SZX;
			fX2 += fX2a;
		}
		fFangle[i] += glAp.sof(fFacc[i]);
		if(fFangle[i] >= 360.0f)fFangle[i] -= 360.0f;
		fFtime[i] += glAp.sof(1);
		if(fFtime[i] >= 15.0f)
		{
			fFtime[i] = 2 + (float)(rand() % 51) / 10.0f;
			fFacc[i] = 90 + (float)(rand() % 91);
		}
		glEnd();
	}
}

GLuint uiSky[6];
char* strSky[] = {"front.bmp", "left.bmp", "back.bmp", "right.bmp", "top.bmp"};

//====================

GLvoid loadSky()
{
	FOR(i, 5)
	{
		string strPath = "Textures\\sky\\";
		strPath += strSky[i];
		textureBMP(&uiSky[i], (char*)strPath.c_str(), true, false, MIPMAP);
	}
	textureBMP(&uiFlagTex, "Textures\\flag.bmp", true, true, LINEAR);
}

//====================

GLvoid renderSky(CVector3 vCenter)
{
	GLfloat x = vCenter.x, y = vCenter.y, z = vCenter.z;
	glDepthMask(0);
	glEnable(GL_TEXTURE_2D);
	glColor3ub(255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, uiSky[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(x - 20, y + 20, z - 20);
		glTexCoord2f(1, 1); glVertex3f(x + 20, y + 20, z - 20);
		glTexCoord2f(1, 0); glVertex3f(x + 20, y, z - 20);
		glTexCoord2f(0, 0); glVertex3f(x - 20, y, z - 20);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, uiSky[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(x + 20, y + 20, z - 20);
		glTexCoord2f(1, 1); glVertex3f(x + 20, y + 20, z + 20);
		glTexCoord2f(1, 0); glVertex3f(x + 20, y, z + 20);
		glTexCoord2f(0, 0); glVertex3f(x + 20, y, z - 20);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, uiSky[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(x + 20, y + 20, z + 20);
		glTexCoord2f(1, 1); glVertex3f(x - 20, y + 20, z + 20);
		glTexCoord2f(1, 0); glVertex3f(x - 20, y, z + 20);
		glTexCoord2f(0, 0); glVertex3f(x + 20, y, z + 20);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, uiSky[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(x - 20, y + 20, z + 20);
		glTexCoord2f(1, 1); glVertex3f(x - 20, y + 20, z - 20);
		glTexCoord2f(1, 0); glVertex3f(x - 20, y, z - 20);
		glTexCoord2f(0, 0); glVertex3f(x - 20, y, z + 20);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, uiSky[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f(x - 20, y + 20, z - 20);
		glTexCoord2f(1, 1); glVertex3f(x + 20, y + 20, z - 20);
		glTexCoord2f(0, 1); glVertex3f(x + 20, y + 20, z + 20);
		glTexCoord2f(0, 0); glVertex3f(x - 20, y + 20, z + 20);
	glEnd();

	glDepthMask(1);
}