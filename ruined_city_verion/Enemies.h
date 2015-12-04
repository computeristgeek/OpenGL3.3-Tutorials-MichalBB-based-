class CEnemy
{
public:
	CVector3 vPos;
	GLfloat fLife, fFade;
	
	virtual GLvoid render() = 0;

	CVector3** vColBox;
	GLint iNumCB;
	int* iNumPoints;
	CEnemy(){fFade = 1.0f;}
};

class CTower : public CEnemy
{
public:
	GLfloat fAngle, fTime, fTime2;
	GLboolean bRotating, bPlaySnd;
	GLvoid render();
	struct CSmoke{CVector3 vPos; BYTE bColor; GLfloat fLife;};
	vector<CSmoke> sSmoke;
	GLvoid renderSmoke();
	CTower(){fAngle = 0.0f; bRotating = false; bPlaySnd = true; fTime = (float)(rand() % 31) * 0.1f; fTime2 = 0.0f;}
};

class CSpider : public CEnemy
{
public:
	CVector3 vLastPos;
	GLfloat fTime, fDieAngleY, fDieAngleX, fGrav;
	GLboolean bAttack, bDying;
	GLint iType;
	CAnimData aData;
	GLvoid render();
	vector<CVector3> vBPos, vBAcc;
	GLvoid renderBlood();
	GLvoid addBlood(CVector3 vPos, GLfloat fDamage);
	CSpider(){fGrav = 0.0f; fTime = (float)(rand() % 16) * 0.1f; bAttack = false; fDieAngleX = 0.0f; bDying = false;}
};

class CSpiderWeb
{
public:
	struct CSpiderBall
	{
		CVector3 vPos, vAcc;
		GLfloat fAngle;
		GLfloat fTime;
	};
	CVector3 vPoints[4];
	CVector3* vNests;
	vector<CSpiderBall> sbBalls;
	GLfloat fTime;
	GLint iNests, *iNSpiders, *iNAll, iAll, iType;
	GLvoid render();
};

extern CMSModel* mEnemies;

extern vector<CTower> tTowers;
extern vector<CSpider> sSpiders;
extern vector<CSpiderWeb> swWebs;

extern UINT uiSWTex;
extern CVector2 vCCoord[4]; // Classic coordinates

GLvoid loadEnemies(GLint iNModels);
GLvoid renderEnemies();
GLvoid renderBlendedEnemyStuff();

GLint collisionLineEnemies(CVector3 vA, CVector3 vB);
GLvoid takeEnemiesLife(GLfloat fDamage);