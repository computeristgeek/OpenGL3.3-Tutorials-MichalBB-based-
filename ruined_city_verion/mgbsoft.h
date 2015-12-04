#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

#define FOR(i, q)for(GLint i = 0; i < q; i++)
#define ESZ(elem)(int)elem.size()

#define PI 3.14159265358979323846f

namespace mgbsoft
{
//''''' CVECTOR2 '''''

class CVector2
{
public:
	GLfloat x, y; // Data used for position on axises
	//----------------- Constructors -----------------------

	CVector2();
	CVector2(GLfloat X, GLfloat Y);

	//==================== Operators -----------------------

	CVector2 operator+(CVector2 vAdd);
	CVector2 operator-(CVector2 vSub);
	CVector2 operator*(GLfloat fValue);
	CVector2 operator/(GLfloat fValue);
	GLvoid operator+=(CVector2 vAdd);
	GLvoid operator-=(CVector2 vSub);
	GLvoid operator*=(GLfloat fValue);
	GLvoid operator/=(GLfloat fValue);
};

//''''' CVECTOR3 '''''

class CVector3
{
public:
	GLfloat x, y, z; // Data used for position on axises

	//----------------- Constructors -----------------------
	
	CVector3();
	CVector3(GLfloat X, GLfloat Y, GLfloat Z);

	//==================== Operators -----------------------

	CVector3 operator+(CVector3 vAdd);
	CVector3 operator-(CVector3 vSub);
	CVector3 operator*(GLfloat fValue);
	CVector3 operator/(GLfloat fValue);
	GLvoid operator+=(CVector3 vAdd);
	GLvoid operator-=(CVector3 vSub);
	GLvoid operator*=(GLfloat fValue);
	GLvoid operator/=(GLfloat fValue);
};

class CApplication
{
public:
	GLuint hWindow; // Handle to a window
	HDC hDC; // Window's device context
	HGLRC hRC; // Rendering context
	HINSTANCE hInstance; // CApplication's instance

	char* appName;
	HANDLE hMutex;

	GLint scrX, scrY, iBpp;
	DEVMODE dmSettings;
	GLboolean fullscreen; // To check if app is in fullscreen mode

	GLboolean active; // To check if app is active
	DWORD frameTime, lastTime;
	GLfloat frameInterval;

	GLint FPS; // How many frames per second are there

	GLvoid (*init)(); // Pointers to main functions
	GLvoid (*glScene)();
	GLvoid (*freeData)();

	GLint initializeApp(char* strName, WNDPROC prFunc);
	GLvoid setBaseFunc(GLvoid (*INIT)(), GLvoid (*GLSCENE)(), GLvoid (*FREEDATA)());
	GLvoid registerAppClass(HINSTANCE appInstance);
	GLvoid createGLWindow(GLint width, GLint height, GLint bpp, GLboolean bFS);
	GLvoid appBody();
	GLvoid shutdown();

	GLvoid resetTimer();

	GLfloat fNear, fFar, fVAngle; // View angle
	GLvoid setGLViewport(GLint iX, GLint iY, GLint iWidth, GLint iHeight, GLfloat sfNear, GLfloat sfFar, GLfloat sfVAngle);
	GLvoid changeDisplaying(GLint scrWidth, GLint scrHeight, GLint bpp, GLboolean bFS);
	GLvoid switchFS();

	GLvoid updateFPS();
	GLfloat sof(GLfloat value);

	WNDPROC msgFunc;

	GLint key(GLint iKey);
	GLint onekey(GLint iKey);
	GLchar kp[256];

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
	GLint pxsize;
	GLint x, y;
	CFData(){}
	CFData(GLint PXSIZE, GLint X, GLint Y)
	{
		x = X; y = Y; pxsize = PXSIZE;
	}
};

class CFreeType
{
public:
	GLvoid createChar(WORD ch);
	GLvoid createFont(char* fntName, GLint h);
	GLvoid print(CFData fData, char* strText, ...);
	GLvoid wprint(CFData fData, wstring strText);
	GLvoid CFreeType::printwords(vector<wstring> words, CVector2 tl, CVector2 br, GLint pxsize, GLboolean center);
	GLint getxsize(wstring strText, GLint pxsize);
	GLint getcenterx(GLint size, GLint x1, GLint x2);
private:
	FT_Face face;
	GLuGLint chars[383];
	short chsize[383];
	GLuGLint list_base;
	GLfloat height;
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

GLfloat vecMagnitude(CVector3 vVector);
GLvoid vecNormalize(CVector3 &vVector);
CVector3 vecCross(CVector3 vVector1, CVector3 vVector2);
GLfloat vecDist(CVector3 vPoint1, CVector3 vPoint2);
GLfloat vecDot(CVector3 vVector1, CVector3 vVector2);
double vecAngle(CVector3 vVector1, CVector3 vVector2);

GLboolean collisionLinePoly(CVector3 vA, CVector3 vB, CVector3* vNormal, CVector3 vPoly[], GLint iNumVerts);
GLboolean collisionSphereLine(CVector3 vCenter, GLfloat fRad, CVector3 vA, CVector3 vB);
GLboolean collisionSpherePoly(CVector3 vPoly[], GLint iNumVerts, CVector3 *vNormal, CVector3 vCenter,  GLfloat fRad, GLfloat &fDFCenter);

CVector3 getLastIntersection();
CVector3 closestPointOnLine(CVector3 vA, CVector3 vB, CVector3 vPoint);
// Texture creation and GL utilities

#define NEAREST 0
#define LINEAR 1
#define MIPMAP 2

GLvoid textureBMP(GLuint *uiStor, char* strFileName, GLboolean bClamp, GLboolean bTrans, GLuint uiFilter);
GLvoid ortho2DBegin(GLint w, GLint h);
GLvoid ortho2DEnd();

// Font utility

wchar_t* FT(const char* strText);

vector<wstring> split(wstring str, wstring sep);
vector<wstring> wordsNL(vector<wstring> words);
