#include "common_header.h"

#include "Lin_OpenGLApp.h"

COpenGLWinApp appMain;

/*-----------------------------------------------

Name:	ResetTimer

Params:	none

Result:	Resets application timer (for example
		after re-activation of application).

/*---------------------------------------------*/

GLvoid COpenGLWinApp::ResetTimer()
{
	dwLastFrame = glfwGetTime();
	fFrameInterval = 0.0f;
}

/*-----------------------------------------------

Name:	UpdateTimer

Params:	none

Result:	Updates application timer.

/*---------------------------------------------*/

GLvoid COpenGLWinApp::UpdateTimer()
{
	unsigned long dwCur = glfwGetTime();
	fFrameInterval = float(dwCur-dwLastFrame)*0.001f;
	dwLastFrame = dwCur;
}

/*-----------------------------------------------

Name:	sof

Params:	fVal

Result:	Stands for speed optimized float.

/*---------------------------------------------*/

GLfloat COpenGLWinApp::sof(GLfloat fVal)
{
	return fVal*fFrameInterval;
}

/*-----------------------------------------------

Name:	InitializeApp

Params:	a_sAppName

Result:	Initializes app with specified (unique)
		application identifier.

/*---------------------------------------------*/

GLboolean COpenGLWinApp::InitializeApp(string a_sAppName)
{
	sAppName = a_sAppName;
	glfwInit();
////////hMutex = CreateMutex(NULL, 1, sAppName.c_str());
////////if(GetLastError() == ERROR_ALREADY_EXISTS)
////////{
////////	MessageBox(NULL, "This application already runs!", "Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
////////	return 0;
////////}
	return 1;
}

/*-----------------------------------------------

Name:	RegisterAppClass

Params:	a_hInstance - application instance handle

Result:	Registers application window class.
TODO: Replace Handler or remove

long CALLBACK GlobalMessageHandler(GLuint hWnd, GLuint uiMsg, GLuint wParam, long lParam)
{
	return appMain.MsgHandlerMain(hWnd, uiMsg, wParam, lParam);
}
/*---------------------------------------------*/

GLvoid COpenGLWinApp::RegisterAppClass(GLvoid* a_hInstance)
{
}

/*-----------------------------------------------

Name:	CreateAppWindow

Params:	sTitle - title of created window

Result:	Creates main application window.

/*---------------------------------------------*/

GLboolean COpenGLWinApp::CreateAppWindow(string sTitle)
{
	hWnd = glfwCreateWindow(800, 600, sTitle.c_str(), glfwGetPrimaryMonitor(),NULL);

	if(!oglControl.InitOpenGL(hInstance, hWnd, 3, 3, InitScene, RenderScene, NULL, &oglControl))return false;

	if (!hWnd)
	{
		MessageBox(*hWnd, "Could Not Set Fullscreen", "Fullscreen Error", MB_ICONERROR);
		return false;
	}
	return true;
}

/*-----------------------------------------------

Name:	AppBody

Params:	none

Result:	Main application body infinite loop.

/*---------------------------------------------*/

GLvoid COpenGLWinApp::AppBody()
{
	while(!glfwWindowShouldClose(hWnd))
	{
		if(glfwGetKey(hWnd, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
		{
			MessageBox(hWnd,"ESC PRESSED","Normal Exit",NULL);
			return;
		}
		else 
		{
			UpdateTimer();
			oglControl.Render(&oglControl);
		}
		//else Sleep(200); // Do not consume processor power if application isn't active
		glfwPollEvents();
	}
}

/*-----------------------------------------------

Name:	Shutdown

Params:	none

Result:	Shuts down application and releases used
		memory.

/*---------------------------------------------*/

GLvoid COpenGLWinApp::Shutdown()
{
	oglControl.ReleaseOpenGLControl(&oglControl);

	glfwDestroyWindow(hWnd);
	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
	//ReleaseMutex(hMutex);
	glfwTerminate();
}

/*-----------------------------------------------

Name:	MsgHandlerMain

Params:	windows message stuff

Result:	Application messages handler.


long CALLBACK COpenGLWinApp::MsgHandlerMain(GLuint hWnd, GLuint uiMsg, GLuint wParam, long lParam)
{
	PAINTSTRUCT ps;

	switch(uiMsg)
	{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);					
			EndPaint(hWnd, &ps);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_ACTIVATE:
		{
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					bAppActive = true;
					break;
				case WA_INACTIVE:
					bAppActive = false;
					break;
			}
			break;
		}

		case WM_SIZE:
			oglControl.ResizeOpenGLViewportFull();
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}
/*---------------------------------------------*/

/*-----------------------------------------------

Name:		getInstance

Params:	none

Result:	Returns application instance.

/*---------------------------------------------*/

GLvoid* COpenGLWinApp::GetInstance()
{
	return hInstance;
}

/*-----------------------------------------------

Name:		msgHandlerMain

Params:	whatever

Result:	Application messages handler.

/*---------------------------------------------*/

//GLint main(GLvoid* hInstance, GLvoid* hPrevInstance, LPSTR sCmdLine, GLint iShow)
int main(int argc, char** argv)
{
	if(!appMain.InitializeApp("01_opengl_3_3"))
		return 0;
	appMain.RegisterAppClass(&appMain);

	if(!appMain.CreateAppWindow("01.) Creating OpenGL 3.3 Window - Tutorial by Michal Bubnar (www.mbsoftworks.sk)"))
		return 0;
	appMain.ResetTimer();

	appMain.AppBody();
	appMain.Shutdown();

	return 0;
}

