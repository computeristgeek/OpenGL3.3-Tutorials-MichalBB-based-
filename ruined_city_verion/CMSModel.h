class CGroup
{
public:
	WORD wNFaces;
	char cMIndex;
	BYTE bBlend;
	CVector3* vVerts;
	CVector3* vNorms3;
	CVector3* vNorms;
	CVector2* vCoords;
};

class CAnimation
{
public:
	string strName;
	BYTE bData, bNumKF;
	float* fTime;
	BYTE* bKFData;
	BYTE** bBitFld;
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
	int iKF, iAnim;
	float fBTime, fSTime; // Time saved when pausing
	int iLoop, iLoops; // Loops already passed
	bool* bCanRetKFP; // If it is able to return that some keyframe is passed
	bool bPaused, bPauseAtEnd;

	bool kfPassed(int iKFrame);
	void pauseAnim(), continueAnim();

	CAnimData(){iAnim = -1;}
};

class CMSModel
{
public:

	bool bLoaded; // First of all, check if the model has been loaded properly

	WORD wNGroups, wNAnims;
	CGroup* gGroups;
	CAnimation* aAnims;
	float fgBlend;

	void startAnimation(char* strName, int iLoop, CAnimData *aData);
	
	void loadModelData(char* strFile);
	void renderModel(CAnimData* aData);
	void setGlobalBlend(float sfgBlend);

	CMSModel::CMSModel(){bLoaded = false; fgBlend = 1.0f;}
};