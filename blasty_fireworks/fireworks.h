class CRomanLighter
{
public:
	CVector3 vPos, vDir;
	GLint iPartLeft;
	vector<CParticle> pBombs;
	GLfloat fWait, fFade;

	CRomanLighter(){fWait = 2.0f; fFade = 1.0f;}
	GLvoid renderAndUpdate();
};

//====================

class CBlastyBlaster
{
public:
	CVector3 vPos, vDir;
	GLint iPartLeft, iPos;
	vector<CParticle> pBombs;
	GLfloat fWait, fFade;

	CBlastyBlaster(){fWait = 2.0f; fFade = 1.0f; iPos = 0;}
	GLvoid renderAndUpdate();
};

//====================

class CVolcano
{
public:
	CVector3 vPos, vDir;
	GLint iUp;
	vector<CParticle> pParts;
	GLfloat fWait, fFade, fHeight;

	CVolcano(){fWait = 2.5f; fFade = 1.0f; fHeight = 0.0f; iUp = 1;}
	GLvoid renderAndUpdate();
};

//====================

extern vector<CParticle> pParts;

GLvoid createParticles(CVector3 vPos, GLfloat fMaxSpeed, GLint iCount, GL_UNSIGNED_BYTE* bColor);
GLfloat randValue(GLfloat fMin, GLfloat fMax);
GLvoid renderAllParticles(vector<CParticle> &pParts, GLint iFade);

GLvoid loadFireworksModels();
GLvoid updateFireworks();