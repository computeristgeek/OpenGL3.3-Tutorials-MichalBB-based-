#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

#define FOR(i, q)for(int i = 0; i < q; i++)
#define ESZ(elem)(int)elem.size()

#define PI 3.14159265358979323846f

namespace mgbsoft
{
//''''' CVECTOR2 '''''

class CVector2
{
public:
	float x, y; // Data used for position on axises
	//----------------- Constructors -----------------------

	CVector2();
	CVector2(float X, float Y);

	//==================== Operators -----------------------

	CVector2 operator+(CVector2 vAdd);
	CVector2 operator-(CVector2 vSub);
	CVector2 operator*(float fValue);
	CVector2 operator/(float fValue);
	void operator+=(CVector2 vAdd);
	void operator-=(CVector2 vSub);
	void operator*=(float fValue);
	void operator/=(float fValue);
};

//''''' CVECTOR3 '''''

class CVector3
{
public:
	float x, y, z; // Data used for position on axises

	//----------------- Constructors -----------------------
	
	CVector3();
	CVector3(float X, float Y, float Z);

	//==================== Operators -----------------------

	CVector3 operator+(CVector3 vAdd);
	CVector3 operator-(CVector3 vSub);
	CVector3 operator*(float fValue);
	CVector3 operator/(float fValue);
	void operator+=(CVector3 vAdd);
	void operator-=(CVector3 vSub);
	void operator*=(float fValue);
	void operator/=(float fValue);
};

class CApplication
{
public:
	HWND hWindow; // Handle to a window
	HDC hDC; // Window's device context
	HGLRC hRC; // Rendering context
	HINSTANCE hInstance; // CApplication's instance

	char* appName;
	HANDLE hMutex;

	int scrX, scrY, iBpp;
	DEVMODE dmSettings;
	bool fullscreen; // To check if app is in fullscreen mode

	bool active; // To check if app is active
	DWORD frameTime, lastTime;
	float frameInterval;

	int FPS; // How many frames per second are there

	void (*init)(); // Pointers to main functions
	void (*glScene)();
	void (*freeData)();

	int initializeApp(char* strName, WNDPROC prFunc);
	void setBaseFunc(void (*INIT)(), void (*GLSCENE)(), void (*FREEDATA)());
	void registerAppClass(HINSTANCE appInstance);
	void createGLWindow(int width, int height, int bpp, bool bFS);
	void appBody();
	void shutdown();

	void resetTimer();

	float fNear, fFar, fVAngle; // View angle
	void setGLViewport(int iX, int iY, int iWidth, int iHeight, float sfNear, float sfFar, float sfVAngle);
	void changeDisplaying(int scrWidth, int scrHeight, int bpp, bool bFS);
	void switchFS();

	void updateFPS();
	float sof(float value);

	WNDPROC msgFunc;

	int key(int iKey);
	int onekey(int iKey);
	char kp[256];

	CApplication()
	{
		FOR(i, 256)kp[i] = 0;
		init = glScene = freeData = NULL;
		fNear = 0.1f; fFar = 150.0f;
		fVAngle = 45.0f;
	}
};

struct CFData
{
	int pxsize;
	int x, y;
	CFData(){}
	CFData(int PXSIZE, int X, int Y)
	{
		x = X; y = Y; pxsize = PXSIZE;
	}
};

class CFreeType
{
public:
	void createChar(WORD ch);
	void createFont(char* fntName, int h);
	void print(CFData fData, char* strText, ...);
	void wprint(CFData fData, wstring strText);
	void CFreeType::printwords(vector<wstring> words, CVector2 tl, CVector2 br, int pxsize, bool center);
	int getxsize(wstring strText, int pxsize);
	int getcenterx(int size, int x1, int x2);
private:
	FT_Face face;
	GLuint chars[383];
	short chsize[383];
	GLuint list_base;
	float height;
};

}

typedef mgbsoft::CVector2 CVector2;
typedef mgbsoft::CVector3 CVector3;
typedef mgbsoft::CApplication CApplication;
typedef mgbsoft::CFData CFData;
typedef mgbsoft::CFreeType CFreeType;

// 3D Math functions

#define BEHIND 0
#define INTERSECTS 1
#define FRONT 2

float vecMagnitude(CVector3 vVector);
void vecNormalize(CVector3 &vVector);
CVector3 vecCross(CVector3 vVector1, CVector3 vVector2);
float vecDist(CVector3 vPoint1, CVector3 vPoint2);
float vecDot(CVector3 vVector1, CVector3 vVector2);
double vecAngle(CVector3 vVector1, CVector3 vVector2);

bool collisionLinePoly(CVector3 vA, CVector3 vB, CVector3* vNormal, CVector3 vPoly[], int iNumVerts);
bool collisionSphereLine(CVector3 vCenter, float fRad, CVector3 vA, CVector3 vB);
bool collisionSpherePoly(CVector3 vPoly[], int iNumVerts, CVector3 *vNormal, CVector3 vCenter,  float fRad, float &fDFCenter);

CVector3 getLastIntersection();
CVector3 closestPointOnLine(CVector3 vA, CVector3 vB, CVector3 vPoint);
// Texture creation and GL utilities

#define NEAREST 0
#define LINEAR 1
#define MIPMAP 2

void textureBMP(UINT *uiStor, char* strFileName, bool bClamp, bool bTrans, UINT uiFilter);
void ortho2DBegin(int w, int h);
void ortho2DEnd();

// Font utility

wchar_t* FT(const char* strText);

vector<wstring> split(wstring str, wstring sep);
vector<wstring> wordsNL(vector<wstring> words);
