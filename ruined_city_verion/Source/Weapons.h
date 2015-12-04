class CWeapon
{
public:
	int iAmmo, iMaxAmmo;
	CAnimData adWeapon;
	CMSModel mWeapon; string strFile;
	bool bShooting;

	string strName;
	BYTE bR, bG, bB;

	virtual void renderWeapon() = 0;
	virtual void shoot() = 0;
	virtual void renderAmmo() = 0;
	virtual void renderBlended() = 0;
};

class CRipper : public CWeapon
{
public:
	class CRipperDisc
	{
	public:
		CVector3 vPos, vLastPos, vDir, vNext;
		int iFIndex, iNumHits;
		float fAngle, fTime;
		float fAnY, fAnX;
	};
	vector<CRipperDisc> rdDiscs;
	CMSModel mRDisc;
	bool bLoaded;

	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};

class CBullet
{
	public:
		CVector3 vPos, vDir, vEnd;
		float fLife;
};

class CDoubleUzi : public CWeapon
{
public:
	float fZ[2];
	vector<CBullet> bBullets;
	CAnimData adWeapon2;
	int iAUzi; // Active uzi
	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
	CDoubleUzi(){iAUzi = 0; fZ[0] = fZ[1] = 0.0f;}
};

class CQuadMinigun : public CWeapon
{
public:
	CMSModel mRotator, mCannon;
	float fAngle[4], fSpeed[4], fTime[4], fMAngle, fMSpeed;
	CAnimData adFire[4];
	int iPhase, iChannel;
	bool bLoaded, bOverheat;
	float fOverheat;
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
	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};

class CCannonBall
{
public:
	CVector3 vPos, vLastPos, vAcc;
	bool bExploding;
	float fExpSize, fLife, fDamage;
	CCannonBall(){bExploding = false; fExpSize = 0.0f; fDamage = 400.0f; fLife = 5.0f;}
};

class CCannon : public CWeapon
{
public:
	vector<CCannonBall> bBalls;
	float fShake, fBack, fTime;
	bool bRecover, bShoot;

	CCannon(){fShake = 0.0f; fBack = fTime = 0.0f; bRecover = bShoot = false;}

	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};

class CFlakCannon : public CWeapon
{
public:
	class CFlak
	{
	public:
		float fLife;
		CVector3 vPos;
		int iCurNode, iAllNodes;
		CVector3 vNodes[5];
		CVector3 vDir[5];
	};
	vector<CFlak> fShells;
	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};

class CShockRifle : public CWeapon
{
public:
	class CImpact
	{
	public:
		CVector3 vPos, vDirA, vDirB, vStart;
		float fSize;
		char cOnlyBeam;
	};
	float fTime;
	vector<CImpact> iImpact;
	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};

class CPlasmaThrower : public CWeapon
{
public:

	int iPhase;
	float fUp;
	struct CPlasma
	{
		CVector3 vPos, vLastPos, vDir, vEnd, vUp, vRight;
		float fAngle, fLife;
	};

	vector<CPlasma> pPlasmas;
	CPlasmaThrower(){fUp = 0.0f;}

	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};


class CLightingGun : public CWeapon
{
public:
	void renderWeapon();
	void shoot();
	void renderAmmo();
	void renderBlended();
};

extern int iCurWeapon;
extern int iLIF, iLIE, iEnType;
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

void loadWeapons();
void processWeapons();
void renderWeaponsOrtho();

void renderBox(CVector3 vPos);

extern UINT uiExpTex, uiPartTex, uiSmokeTex;
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
