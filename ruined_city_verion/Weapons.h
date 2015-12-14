class CWeapon
{
public:
	GLint iAmmo, iMaxAmmo;
	CAnimData adWeapon;
	CMSModel mWeapon; string strFile;
	GLboolean bShooting;

	string strName;
	GL_UNSIGNED_BYTE bR, bG, bB;

	virtual GLvoid renderWeapon() = 0;
	virtual GLvoid shoot() = 0;
	virtual GLvoid renderAmmo() = 0;
	virtual GLvoid renderBlended() = 0;
};

class CRipper : public CWeapon
{
public:
	class CRipperDisc
	{
	public:
		CVector3 vPos, vLastPos, vDir, vNext;
		GLint iFIndex, iNumHits;
		GLfloat fAngle, fTime;
		GLfloat fAnY, fAnX;
	};
	vector<CRipperDisc> rdDiscs;
	CMSModel mRDisc;
	GLboolean bLoaded;

	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};

class CBullet
{
	public:
		CVector3 vPos, vDir, vEnd;
		GLfloat fLife;
};

class CDoubleUzi : public CWeapon
{
public:
	GLfloat fZ[2];
	vector<CBullet> bBullets;
	CAnimData adWeapon2;
	GLint iAUzi; // Active uzi
	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
	CDoubleUzi(){iAUzi = 0; fZ[0] = fZ[1] = 0.0f;}
};

class CQuadMinigun : public CWeapon
{
public:
	CMSModel mRotator, mCannon;
	GLfloat fAngle[4], fSpeed[4], fTime[4], fMAngle, fMSpeed;
	CAnimData adFire[4];
	GLint iPhase, iChannel;
	GLboolean bLoaded, bOverheat;
	GLfloat fOverheat;
	CQuadMinigun()
	{
		FOR(i, 4)
		{
			fSpeed[i] = -90.0f * (float)i;
			fAngle[i] = 0.0f;
			fTime[i] = 0.0f;
			fMAngle = fMSpeed = 0.0f;
		}
		iPhase = 0;
		bLoaded = false;
		bOverheat = false;
		fOverheat = 0.0f;
		iChannel = -1;
	}
	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};

class CCannonBall
{
public:
	CVector3 vPos, vLastPos, vAcc;
	GLboolean bExploding;
	GLfloat fExpSize, fLife, fDamage;
	CCannonBall(){bExploding = false; fExpSize = 0.0f; fDamage = 400.0f; fLife = 5.0f;}
};

class CCannon : public CWeapon
{
public:
	vector<CCannonBall> bBalls;
	GLfloat fShake, fBack, fTime;
	GLboolean bRecover, bShoot;

	CCannon(){fShake = 0.0f; fBack = fTime = 0.0f; bRecover = bShoot = false;}

	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};

class CFlakCannon : public CWeapon
{
public:
	class CFlak
	{
	public:
		GLfloat fLife;
		CVector3 vPos;
		GLint iCurNode, iAllNodes;
		CVector3 vNodes[5];
		CVector3 vDir[5];
	};
	vector<CFlak> fShells;
	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};

class CShockRifle : public CWeapon
{
public:
	class CImpact
	{
	public:
		CVector3 vPos, vDirA, vDirB, vStart;
		GLfloat fSize;
		GLchar cOnlyBeam;
	};
	GLfloat fTime;
	vector<CImpact> iImpact;
	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};

class CPlasmaThrower : public CWeapon
{
public:

	GLint iPhase;
	GLfloat fUp;
	struct CPlasma
	{
		CVector3 vPos, vLastPos, vDir, vEnd, vUp, vRight;
		GLfloat fAngle, fLife;
	};

	vector<CPlasma> pPlasmas;
	CPlasmaThrower(){fUp = 0.0f;}

	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};


class CLightingGun : public CWeapon
{
public:
	GLvoid renderWeapon();
	GLvoid shoot();
	GLvoid renderAmmo();
	GLvoid renderBlended();
};

extern GLint iCurWeapon;
extern GLint iLIF, iLIE, iEnType;
CVector3 getIntersection(CVector3 vFrom, CVector3 vTo);
CVector3 getIntersectionEnemy(CVector3 vFrom, CVector3 vTo, CVector3 vBest);
pair<CVector3, CVector3> getNormQuad(CVector3 vNorm);

extern CRipper gRipper;
extern CDoubleUzi gDUzi;
extern CQuadMinigun gQMinigun;
extern CCannon gCannon;
extern CFlakCannon gFCannon;
extern CShockRifle gShockRifle;
extern CLightingGun gLGun;

GLvoid loadWeapons();
GLvoid processWeapons();
GLvoid renderWeaponsOrtho();

GLvoid renderBox(CVector3 vPos);

extern GLuint uiExpTex, uiPartTex, uiSmokeTex;
extern CPlasmaThrower gPlasmaGun;

#define ALLWEAPONS 7

#define DOUBLEUZI 0
#define RIPPER 1
#define SHOCKRIFLE 2
#define QMINIGUN 3
#define FLAKCANNON 4
#define PLASMATHROWER 5
#define CANNON 6

extern CWeapon* wWeapons[ALLWEAPONS];
