#pragma warning(disable: 4996)

#include "mgbsoft.h"

#include <FMOD/fmod.h>
#include "CMSModel.h"
#include "World.h"
#include "Enemies.h"
#include "Weapons.h"



extern CApplication glAp;
GLvoid glInit(), glScene();

#define PRI(s, i)if(1){char rrr[255]; sprintf(rrr, "%s %d", s, i); MessageBox(NULL, rrr, "Info", MB_ICONINFORMATION | MB_OK);}