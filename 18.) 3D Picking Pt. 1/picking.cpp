#include "common_header.h"

#include "win_OpenGLApp.h"
#include "picking.h"

// This is RGB white in our indexing
#define RGB_WHITE (0xFF | (0xFF<<8) | (0xFF<<16))

/*-----------------------------------------------

Name:    GetColorByIndex

Params:  index - index of color you want to generate

Result:  Returns i-th RGB color.

/*---------------------------------------------*/

glm::vec4 GetColorByIndex(int index)
{
	int r = index&0xFF;
	int g = (index>>8)&0xFF;
	int b = (index>>16)&0xFF;

	return glm::vec4(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f, 1.0f);
}

/*-----------------------------------------------

Name:    GetIndexByColor

Params:  r, g, b - RGB values of color

Result:  Kind of inverse to previous function,
		 gets index from selected color.

/*---------------------------------------------*/

int GetIndexByColor(int r, int g, int b)
{
	return (r)|(g<<8)|(b<<16);
}

/*-----------------------------------------------

Name:    GetColorByIndex

Params:  index - index of color you want to generate

Result:  Returns i-th RGB color.

/*---------------------------------------------*/

int GetPickedColorIndexUnderMouse()
{
	POINT mp; GetCursorPos(&mp);
	ScreenToClient(appMain.hWnd, &mp);
	RECT rect; GetClientRect(appMain.hWnd, &rect);
	mp.y = rect.bottom-mp.y;
	BYTE bArray[4];
	glReadPixels(mp.x, mp.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
	int iResult = GetIndexByColor(bArray[0], bArray[1], bArray[2]);
	if(iResult == RGB_WHITE)return -1;
	return iResult;
}