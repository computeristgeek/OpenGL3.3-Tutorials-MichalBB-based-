#include "mgbsoft.h"
#include <algorithm>
#include <commctrl.h>
#include "resource.h"
#include "objects.h"
#include "fireworks.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "mgbsoft.lib")

//====================

LRESULT CALLBACK dlgStartup(GLuint hWnd, GLuint uMessage, WPARAM wParam, LPARAM lParam);
extern GLint iStartRes;
extern CApplication::DispMode dm;

extern CApplication glAp;
extern CFreeType ft;

//====================

GLvoid glInit(), glScene(), freeEverything();