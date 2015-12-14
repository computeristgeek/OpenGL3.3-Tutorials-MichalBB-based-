class CGroup
{
public:
	WORD wNFaces;
	GLchar cMIndex;
	GL_UNSIGNED_BYTE bBlend;
	CVector3* vVerts;
	CVector3* vNorms3;
	CVector3* vNorms;
	CVector2* vCoords;
};

class CAnimation
{
public:
	string strName;
	GL_UNSIGNED_BYTE bData, bNumKF;
	float* fTime;
	GL_UNSIGNED_BYTE* bKFData;
	GL_UNSIGNED_BYTE** bBitFld;
	char** cTexture;
	float** fBlend;
	
	CVector3*** vVerts;
	CVector3*** vNorms;
	CVector2*** vCoords;

	CVector3*** ptrVert1, ***ptrVert2, ***ptrNorm1, ***ptrNorm2;
	CVector2*** ptrCoord1, ***ptrCoord2;
};

class CAnimData
{
public:
	GLint iKF, iAnim;
	GLfloat fBTime, fSTime; // Time saved when pausing
	GLint iLoop, iLoops; // Loops already passed
	bool* bCanRetKFP; // If it is able to return that some keyframe is passed
	GLboolean bPaused, bPauseAtEnd;

	GLboolean kfPassed(GLint iKFrame);
	GLvoid pauseAnim(), continueAnim();

	CAnimData(){iAnim = -1;}
};

class CMSModel
{
public:

	GLboolean bLoaded; // First of all, check if the model has been loaded properly

	WORD wNGroups, wNAnims;
	CGroup* gGroups;
	CAnimation* aAnims;
	GLfloat fgBlend;

	GLvoid startAnimation(char* strName, GLint iLoop, CAnimData *aData);
	
	GLvoid loadModelData(char* strFile);
	GLvoid renderModel(CAnimData* aData);
	GLvoid setGlobalBlend(GLfloat sfgBlend);

	CMSModel::CMSModel(){bLoaded = false; fgBlend = 1.0f;}
};