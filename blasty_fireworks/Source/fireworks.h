class CRomanLighter
{
public:
	CVector3 vPos, vDir;
	int iPartLeft;
	vector<CParticle> pBombs;
	float fWait, fFade;

	CRomanLighter(){fWait = 2.0f; fFade = 1.0f;}
	void renderAndUpdate();
};

//====================

class CBlastyBlaster
{
public:
	CVector3 vPos, vDir;
	int iPartLeft, iPos;
	vector<CParticle> pBombs;
	float fWait, fFade;

	CBlastyBlaster(){fWait = 2.0f; fFade = 1.0f; iPos = 0;}
	void renderAndUpdate();
};

//====================

class CVolcano
{
public:
	CVector3 vPos, vDir;
	int iUp;
	vector<CParticle> pParts;
	float fWait, fFade, fHeight;

	CVolcano(){fWait = 2.5f; fFade = 1.0f; fHeight = 0.0f; iUp = 1;}
	void renderAndUpdate();
};

//====================

extern vector<CParticle> pParts;

void createParticles(CVector3 vPos, float fMaxSpeed, int iCount, BYTE* bColor);
float randValue(float fMin, float fMax);
void renderAllParticles(vector<CParticle> &pParts, int iFade);

void loadFireworksModels();
void updateFireworks();