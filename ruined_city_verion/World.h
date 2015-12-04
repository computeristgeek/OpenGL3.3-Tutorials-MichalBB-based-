class CWorld
{
public:
	CVector3* vFaces;
	CVector3* vNorms;
	CVector3* vFlags;
	CVector2* vCoords;
	GLint iNumT; UINT* uiTxt;
	GLint iNumF, iAll;
	GLint *iFacesT;

	vector<int> iStand, iNotStand;

	GLfloat *fFacc, *fFangle, *fFtime, *fAngles;

	GLvoid loadWorld(char* strFile);
	GLvoid renderWorld();
};

extern CWorld lv1;

class CFPSCamera
{
public:
	CVector3 vEye, vView, vEye2, vView2, vUp;
	CVector3 vQuadA, vQuadB;
	CVector3 vAcc;
	GLfloat fHeight, fSense, fSpeed, fBobAn; // fSense from 1 to 100 (optimum is 50)
	GLboolean bStanding;

	GLvoid moveByMouse();
	GLvoid update();
	GLvoid look();

	GLvoid setMoving(GLint siForw, GLint siBack, GLint siLeft, GLint siRight);
	GLvoid resetMouse();

	GLvoid checkCollision(CVector3 vMoveBy);

	GLfloat getAngleX(), getAngleY();

	CFPSCamera(){};
	CFPSCamera(CVector3 svEye, CVector3 svView, CVector3 svUp, GLfloat sfHeight, GLfloat sfSense, GLfloat sfSpeed);

private:
	POINT pMPCur;
	GLint iForw, iBack, iLeft, iRight;
};

extern CFPSCamera cCam;
extern CFreeType ft;


GLvoid loadSky();
GLvoid renderSky(CVector3 vCenter);
GLvoid InitFMOD();

extern FSOUND_SAMPLE** sndData;
char* readline(FILE* fp, char* buffer);



extern GLfloat afps, nnum;