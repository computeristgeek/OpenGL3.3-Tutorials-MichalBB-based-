#include "mainheader.h"

CFreeType ft;
CWorld lv1;
CFPSCamera cCam = CFPSCamera(CVector3(74, 30, -70), CVector3(74, 30, -71), CVector3(0, 1, 0), 70.0f, 2.0f, 20.0f);

vector<CVector3> vRipper, vRDir;
vector<float> fRAn;
#define CONSTSPEED 150.0f

GLfloat afps, nnum;

GLvoid glInit()
{
	glEnable(GL_DEPTH_TEST); // Enables depth-testing
	glEnable(GL_TEXTURE_2D); // Enables texturing
	glShadeModel(GL_SMOOTH); // Enables smooth shading
	glDepthFunc(GL_LESS);	// The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations

	ft.createFont("Font.ttf", 32);
/*
	FILE* fp = fopen("Textures\\World2.msm", "rb");
	char strLine[50]; fgets(strLine, 50, fp);

	WORD iNMat, wNGroups, wNAnims; fread(&wNGroups, sizeof(WORD), 1, fp);
	fread(&iNMat, sizeof(WORD), 1, fp);
	fread(&wNAnims, sizeof(WORD), 1, fp);
	vector< vector<CVector3> > vPoints;
	vector< vector<CVector2> > vCoords;
	vPoints.resize(iNMat);
	vCoords.resize(iNMat);
	CVector3 vScale(3, 3, 3);
	CVector2 vSss(1, 1);
	FOR(i, wNGroups)
	{
		fgets(strLine, 255, fp); // Name of group, just ignore
		char cMIndex; fread(&cMIndex, sizeof(char), 1, fp);
		BYTE bBlend; fread(&bBlend, sizeof(BYTE), 1, fp);
		WORD wNFaces; fread(&wNFaces, sizeof(WORD), 1, fp);
		FOR(j, wNFaces)
		{
			FOR(k, 3)
			{
				CVector3 bod; fread(&bod, sizeof(CVector3), 1, fp);
				bod.x *= vScale.x; bod.y *= vScale.y; bod.z *= vScale.z;
				CVector2 cord; fread(&cord, sizeof(CVector2), 1, fp);
				cord.x *= vSss.x; cord.y *= vSss.y;
				if(cMIndex != -1){vPoints[cMIndex].push_back(bod); vCoords[cMIndex].push_back(cord);}
			}
		}
	}

	//-------------------
	vector<string> namez;
	FOR(i, iNMat)
	{
		fgets(strLine, 255, fp);
		string strName = strLine; strName = strName.substr(0, strName.length() - 1);
		char trans; fread(&trans, sizeof(char), 1, fp);
		namez.push_back(strName);
	}	
	fclose(fp);
	fp = fopen("City.rcw", "wt");
	char aaa[222]; sprintf(aaa, "TEXTURES %d\n", iNMat);
	fputs(aaa, fp);
	FOR(i, iNMat)
	{
		fputs(namez[i].c_str(), fp); fputs("\n", fp);
		sprintf(aaa, "FACES %d\n", ESZ(vPoints[i]) / 3);
		fputs(aaa, fp);
		FOR(j, ESZ(vPoints[i]))
		{
			sprintf(aaa, "%.2f %.2f %.2f %.2f %.2f\n", vCoords[i][j].x, vCoords[i][j].y, vPoints[i][j].x, vPoints[i][j].y, vPoints[i][j].z);
			fputs(aaa, fp);
		}
	}
	fputs("FLAGS 0\n\n", fp);
	fclose(fp);
*/
	lv1.loadWorld("City.rcw");
	loadSky();
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	cCam.setMoving('W', 'S', 'A', 'D');
	cCam.resetMouse();

	loadWeapons();
	loadEnemies(8);
	InitFMOD();
}

GLvoid glScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	glLoadIdentity(); // Reset matrix

	cCam.update(); cCam.look();
	renderSky(cCam.vEye);

	lv1.renderWorld();

	processWeapons();
	renderEnemies();
	
	renderBlendedEnemyStuff();
	FOR(i, ALLWEAPONS)wWeapons[i]->renderBlended();

	renderWeaponsOrtho();

	SwapBuffers(glAp.hDC); // Swap drawing buffers

	if(glAp.onekey(VK_F5))glAp.changeDisplaying(1024, 768, 32, glAp.fullscreen);
	if(glAp.onekey(VK_F4))glAp.changeDisplaying(800, 600, 32, glAp.fullscreen);
	if(glAp.onekey(VK_F3))glAp.changeDisplaying(640, 480, 32, glAp.fullscreen);
	if(glAp.onekey(VK_F2))glAp.changeDisplaying(320, 240, 32, glAp.fullscreen);
	if(glAp.onekey(VK_F1))
	{
		glAp.switchFS();
		ShowCursor(!glAp.fullscreen);
	}
	static GLfloat floaf;
	floaf += glAp.sof(1.0f);
	if(floaf > 1.0f)
	{
		floaf = 0.0f;
		afps += (float)glAp.FPS;
		nnum += 1.0f;
	}
}
