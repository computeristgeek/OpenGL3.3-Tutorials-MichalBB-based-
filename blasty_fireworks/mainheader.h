#include "mgbsoft.h"
#include <algorithm>
#include <commctrl.h>
#include "resource.h"
#include "objects.h"
#include "fireworks.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "mgbsoft.lib")

//====================

long CALLBACK dlgStartup(GLuint hWnd, GLuint uMessage, GLuint wParam, long lParam);
extern GLint iStartRes;
extern CApplication::DispMode dm;

extern CApplication glAp;
extern CFreeType ft;

//====================

GLvoid glInit(), glScene(), freeEverything();