#include "mainheader.h"

//====================

CApplication glAp;

//====================

LRESULT CALLBACK windowMsgs(GLuint hWindow, GLuint iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch(iMsg)
	{
		case WM_PAINT:									
			BeginPaint(hWindow, &ps);							
			EndPaint(hWindow, &ps);					
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATE: // Watch For Window Activate Message
		{
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					glAp.iActive = 1;
					if(glAp.dm.iFS)glAp.changeDisplaying(glAp.dm.iScrX, glAp.dm.iScrY, glAp.dm.iBpp, 1);
					glAp.resetTimer();
					break;
				case WA_INACTIVE:
					glAp.iActive = 0;
					if(glAp.dm.iFS)ChangeDisplaySettings(NULL, 0);
					break;
			}
			return 0;
		}

		case WM_SIZE:
			glAp.setGLViewport(0, 0, LOWORD(lParam), HIWORD(lParam), -1.0f, -1.0f, -1.0f);
			return 0;

		default:
			return DefWindowProc(hWindow, iMsg, wParam, lParam); // Default window procedure
	}
}

//====================

GLint WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR cmdLine, GLint iShow)
{
	if(!glAp.initializeApp("Blasty Fireworks", (WNDPROC)windowMsgs))return 0;
	DialogBox(hInstance, MAKEINTRESOURCE(ID_STARTUP), NULL, (DLGPROC)dlgStartup);
	if(iStartRes)
	{
		glAp.registerAppClass(hInstance);
		if(!glAp.createGLWindow(dm.iScrX, dm.iScrY, dm.iBpp, dm.iFS))
		{
			MessageBox(NULL, "The window could not be created!", "Fatal error", MB_ICONERROR | MB_OK);
			return 0;
		}
		if(dm.iFS)glAp.changeDisplaying(dm.iScrX, dm.iScrY, dm.iBpp, 1);
		glAp.setBaseFunc(&glInit, &glScene, &freeEverything);
		glAp.setGLViewport(0, 0, -1, -1, 0.1f, 2000.0f, 45.0f);
		glAp.appBody(); // Run main app body
	}
	return 0;
}