class CWorld
{
public:
	CVector3* vFaces;
	CVector3* vNorms;
	CVector3* vFlags;
	CVector2* vCoords;
	int iNumT; UINT* uiTxt;
	int iNumF, iAll;
	int *iFacesT;

	vector<int> iStand, iNotStand;

	float *fFacc, *fFangle, *fFtime, *fAngles;

	void loadWorld(char* strFile);
	void renderWorld();
};

extern CWorld lv1;

class CFPSCamera
{
public:
	CVector3 vEye, vView, vEye2, vView2, vUp;
	CVector3 vQuadA, vQuadB;
	CVector3 vAcc;
	float fHeight, fSense, fSpeed, fBobAn; // fSense from 1 to 100 (optimum is 50)
	bool bStanding;

	void moveByMouse();
	void update();
	void look();

	void setMoving(int siForw, int siBack, int siLeft, int siRight);
	void resetMouse();

	void checkCollision(CVector3 vMoveBy);

	float getAngleX(), getAngleY();

	CFPSCamera(){};
	CFPSCamera(CVector3 svEye, CVector3 svView, CVector3 svUp, float sfHeight, float sfSense, float sfSpeed);

private:
	POINT pMPCur;
	int iForw, iBack, iLeft, iRight;
};

extern CFPSCamera cCam;
extern CFreeType ft;


void loadSky();
void renderSky(CVector3 vCenter);
void InitFMOD();

extern FSOUND_SAMPLE** sndData;
char* readline(FILE* fp, char* buffer);



extern float afps, nnum;