#include "msmmodel.h"

//====================

struct CParticle
{
	CVector3 vPos, vDir;
	GLfloat fFade;
	GL_UNSIGNED_BYTE bColor[3];
};

//====================

class CCamera
{
public:
	CVector3 vEye, vView, vUp;
	GLfloat fSense, fSpeed;

	GLvoid rotateByMouse();
	GLvoid update();
	GLvoid look();

	GLvoid setMoving(GLint siForw, GLint siBack, GLint siLeft, GLint siRight);
	GLvoid resetMouse();

	GLvoid checkCollision();

	GLfloat getAngleX(), getAngleY();

	CCamera(){};
	CCamera(CVector3 svEye, CVector3 svView, CVector3 svUp, GLfloat sfSense, GLfloat sfSpeed);

private:
	POINT pMPCur;
	GLint iForw, iBack, iLeft, iRight;
};

//====================

struct CWorld
{
	vector<CVector3> vVerts;
	vector<CVector3> vNorms;
	vector<CVector2> vCoords;
	vector<GLuint> uiTextures;
	vector<int> iNumFaces;

	CMSMModel* mModels;

	struct CObject
	{
		vector<CVector3> vBoundBox;
		vector<CVector3> vNorms;
		CVector3 vPos;
		GLint iModel;
	};

	vector<CObject> oObjects;
	
	GLvoid loadWorld(char* strFile);
	GLvoid renderWorld();
};

//====================

char* readline(FILE* fp);
string fileName(string strPath);
char* fileDir(char* strFileName);
GLvoid changeDir(char* strNew);

//====================

extern CCamera camMain;
extern vector<pair<string, GLuint> > tTextures;
extern CWorld wWorld;

//====================

GLvoid loadTextures();
