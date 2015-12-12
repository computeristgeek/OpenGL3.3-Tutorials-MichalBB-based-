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
	tLastFrame = clock();
	fFrameInterval = 0.0f;
}

/*-----------------------------------------------

Name:	UpdateTimer

Params:	none

Result:	Updates application timer.

/*---------------------------------------------*/

GLvoid COpenGLWinApp::UpdateTimer()
{
	clock_t tCur = clock();
	fFrameInterval = float(tCur-tLastFrame)/float(CLOCKS_PER_SEC);
	tLastFrame = tCur;
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

	glfwSetErrorCallback(error_CB);

	if(!glfwInit())
	{
		MessageBox(hWnd,"Failed to Initialize GLFW","glfwInit Failure",NULL);
		return GL_FALSE;
	}
	return GL_TRUE;
}

/*-----------------------------------------------

Name:	RegisterAppClass

Params:	a_hInstance - application instance handle

Result:	Registers application window class.

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
	/*-----------------------------MessageBox-------------------*/
	char replyMsg; bool reply=false;
	cout<<"Fullscreen:"<<"Would you like to run in fullscreen?"<<"(yes/No)>"<<endl;cin>>replyMsg; reply=(replyMsg=='y'||replyMsg=='Y'?true:false);
	if(reply)
	{
		hWnd = glfwCreateWindow(800, 600, sTitle.c_str(), glfwGetPrimaryMonitor(),NULL);
	}
	else
	{
		hWnd = glfwCreateWindow(800, 600, sTitle.c_str(), NULL, NULL);
	}
	framebuffer_CB(hWnd,800,600); // to initiate resize for glViewport

	if(!oglControl.InitOpenGL(hInstance, hWnd, 3, 3, InitScene, RenderScene, ReleaseScene, &oglControl))return false;

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
		bAppActive = glfwGetWindowAttrib(hWnd, GLFW_FOCUSED);
		if(bAppActive)
		{
			UpdateTimer();
			oglControl.Render(&oglControl);
		}
		else 
		{
			//else Sleep(200); // Do not consume processor power if application isn't active
		}
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
}

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

int main(int argc, char** argv)
{
	if(!appMain.InitializeApp("01_opengl_3_3"))
		return 1;
	appMain.RegisterAppClass(&appMain);

	if(!appMain.CreateAppWindow("01.) Creating OpenGL 3.3 Window - Tutorial by Michal Bubnar (www.mbsoftworks.sk)"))
		return 1;
	appMain.ResetTimer();
	glfwSetKeyCallback(appMain.hWnd,key_CB);
	glfwSetFramebufferSizeCallback(appMain.hWnd,framebuffer_CB);

	appMain.AppBody();
	appMain.Shutdown();

	glfwTerminate();
	return 0;
}

