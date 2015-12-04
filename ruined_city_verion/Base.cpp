#include "mainheader.h"

//====================

CApplication glAp;

//====================

LRESULT CALLBACK windowMsgs(HWND hWindow, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
					glAp.active = true;
					if(glAp.fullscreen)glAp.changeDisplaying(glAp.scrX, glAp.scrY, glAp.iBpp, glAp.fullscreen);
					glAp.resetTimer();
					break;
				case WA_INACTIVE:
					glAp.active = false;
					if(glAp.fullscreen)ChangeDisplaySettings(NULL, 0);
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

GLvoid freeEverything()
{

}

//====================

GLint WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR cmdLine, GLint iShow)
{
	if(!glAp.initializeApp("Ruined city of Verion", (WNDPROC)windowMsgs))return 0;
	glAp.registerAppClass(hInstance);
	glAp.createGLWindow(800, 600, 32, (MessageBox(NULL, "Would you like to run in a fullscreen mode?", 
		"Fullscreen", MB_YESNO | MB_ICONQUESTION) == IDYES)); // Create window
	glAp.setBaseFunc(&glInit, &glScene, &freeEverything);
	glAp.setGLViewport(0, 0, -1, -1, 0.1f, 2000.0f, 45.0f);
	glAp.appBody(); // Run main app body
	if(nnum == 0.0f)afps = 0.0f;
	else afps /= nnum;
	char aaa[222]; sprintf(aaa, "AVERAGE FPS:\n%.3f", afps);
	MessageBox(NULL, aaa, "INFO", MB_ICONINFORMATION | MB_OK);
	return 0;
}