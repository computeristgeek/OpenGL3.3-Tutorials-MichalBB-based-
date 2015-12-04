#include "msmmodel.h"

//====================

struct CParticle
{
	CVector3 vPos, vDir;
	float fFade;
	BYTE bColor[3];
};

//====================

class CCamera
{
public:
	CVector3 vEye, vView, vUp;
	float fSense, fSpeed;

	void rotateByMouse();
	void update();
	void look();

	void setMoving(int siForw, int siBack, int siLeft, int siRight);
	void resetMouse();

	void checkCollision();

	float getAngleX(), getAngleY();

	CCamera(){};
	CCamera(CVector3 svEye, CVector3 svView, CVector3 svUp, float sfSense, float sfSpeed);

private:
	POINT pMPCur;
	int iForw, iBack, iLeft, iRight;
};

//====================

struct CWorld
{
	vector<CVector3> vVerts;
	vector<CVector3> vNorms;
	vector<CVector2> vCoords;
	vector<UINT> uiTextures;
	vector<int> iNumFaces;

	CMSMModel* mModels;

	struct CObject
	{
		vector<CVector3> vBoundBox;
		vector<CVector3> vNorms;
		CVector3 vPos;
		int iModel;
	};

	vector<CObject> oObjects;
	
	void loadWorld(char* strFile);
	void renderWorld();
};

//====================

char* readline(FILE* fp);
string fileName(string strPath);
char* fileDir(char* strFileName);
void changeDir(char* strNew);

//====================

extern CCamera camMain;
extern vector<pair<string, UINT> > tTextures;
extern CWorld wWorld;

//====================

void loadTextures();
