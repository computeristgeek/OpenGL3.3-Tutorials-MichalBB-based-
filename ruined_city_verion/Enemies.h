class CEnemy
{
public:
	CVector3 vPos;
	float fLife, fFade;
	
	virtual void render() = 0;

	CVector3** vColBox;
	int iNumCB;
	int* iNumPoints;
	CEnemy(){fFade = 1.0f;}
};

class CTower : public CEnemy
{
public:
	float fAngle, fTime, fTime2;
	bool bRotating, bPlaySnd;
	void render();
	struct CSmoke{CVector3 vPos; BYTE bColor; float fLife;};
	vector<CSmoke> sSmoke;
	void renderSmoke();
	CTower(){fAngle = 0.0f; bRotating = false; bPlaySnd = true; fTime = (float)(rand() % 31) * 0.1f; fTime2 = 0.0f;}
};

class CSpider : public CEnemy
{
public:
	CVector3 vLastPos;
	float fTime, fDieAngleY, fDieAngleX, fGrav;
	bool bAttack, bDying;
	int iType;
	CAnimData aData;
	void render();
	vector<CVector3> vBPos, vBAcc;
	void renderBlood();
	void addBlood(CVector3 vPos, float fDamage);
	CSpider(){fGrav = 0.0f; fTime = (float)(rand() % 16) * 0.1f; bAttack = false; fDieAngleX = 0.0f; bDying = false;}
};

class CSpiderWeb
{
public:
	struct CSpiderBall
	{
		CVector3 vPos, vAcc;
		float fAngle;
		float fTime;
	};
	CVector3 vPoints[4];
	CVector3* vNests;
	vector<CSpiderBall> sbBalls;
	float fTime;
	int iNests, *iNSpiders, *iNAll, iAll, iType;
	void render();
};

extern CMSModel* mEnemies;

extern vector<CTower> tTowers;
extern vector<CSpider> sSpiders;
extern vector<CSpiderWeb> swWebs;

extern UINT uiSWTex;
extern CVector2 vCCoord[4]; // Classic coordinates

void loadEnemies(int iNModels);
void renderEnemies();
void renderBlendedEnemyStuff();

int collisionLineEnemies(CVector3 vA, CVector3 vB);
void takeEnemiesLife(float fDamage);