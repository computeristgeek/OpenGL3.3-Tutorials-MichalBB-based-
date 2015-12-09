#include "common_header.h"

#include "Lin_OpenGLApp.h"

COpenGLWinApp appMain;

GLchar Keys::kp[256] = {0};

/*-----------------------------------------------

Name:	Key

Params:	iKey - virtual key code

Result:	Return true if key is pressed.

/*---------------------------------------------*/

GLint Keys::Key(GLint iKey)
{
	return (GetAsyncKeyState(iKey)>>15)&1;
}

/*-----------------------------------------------

Name:	Onekey

Params:	iKey - virtual key code

Result:	Return true if key was pressed, but only
		once (the key must be released).

/*---------------------------------------------*/

GLint Keys::Onekey(GLint iKey)
{
	if(Key(iKey) && !kp[iKey]){kp[iKey] = 1; return 1;}
	if(!Key(iKey))kp[iKey] = 0;
	return 0;
}

/*-----------------------------------------------

Name:	ResetTimer

Params:	none

Result:	Resets application timer (for example
		after re-activation of application).

/*---------------------------------------------*/

GLvoid COpenGLWinApp::ResetTimer()
{
	dwLastFrame = GetTickCount();
	fFrameInterval = 0.0f;
}

/*-----------------------------------------------

Name:	UpdateTimer

Params:	none

Result:	Updates application timer.

/*---------------------------------------------*/

GLvoid COpenGLWinApp::UpdateTimer()
{
	unsigned long dwCur = GetTickCount();
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
	union semun {
	    int                 val;   /* value for SETVAL             */
	    struct semid_ds    *buf;   /* buffer for IPC_STAT, IPC_SET */
	    unsigned short     *array; /* array for GETALL, SETALL     */
	    struct seminfo     __buf;  /* buffer for IPC info          */ 
	}arg;
	key_t 	       semKey;
	int		flag;
	flag = IPC_CREAT;
	if( ( semKey = (key_t) atol( sAppName.c_str() ) ) == 0 )
	{
		cerr<<"This application already runs!"<<"Multiple Instances Found."<<endl;
		return 0;
	}
	flag |= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	int tmp;
	if (tmp=(int) semget( semKey, 1, flag )==EEXIST)
	{
		cerr<<"This application already runs!"<<"Multiple Instances Found."<<endl;
		return 0;
	}
	else hMutex=tmp;
	if (hMutex < 0)
	{
		cerr<<"RC_OBJECT_NOT_CREATED"<<endl;
		return 0;
	}
	arg.val = 1;
	if (semctl(hMutex, 0, SETVAL, arg) == -1)
	{
		cerr<<"RC_OBJECT_NOT_CREATED"<<endl;
		return 0;
	}
	semBuf.sem_num = 0;
	semBuf.sem_op = -1;
	semBuf.sem_flg = SEM_UNDO;
	if (semop(hMutex, &semBuf, 1) != 0)
	{
		cerr<<"RC_LOCK_ERROR"<<endl;
		return 0;
	}
	return 1;
}

/*-----------------------------------------------

Name:	RegisterAppClass

Params:	a_hInstance - application instance handle

Result:	Registers application window class.

/*---------------------------------------------*/

long CALLBACK GlobalMessageHandler(GLuint hWnd, GLuint uiMsg, GLuint wParam, long lParam)
{
	return appMain.MsgHandlerMain(hWnd, uiMsg, wParam, lParam);
}

GLvoid COpenGLWinApp::RegisterAppClass(GLvoid* a_hInstance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = GlobalMessageHandler;
	wcex.lpszClassName = sAppName.c_str();

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}

/*-----------------------------------------------

Name:	CreateAppWindow

Params:	sTitle - title of created window

Result:	Creates main application window.

/*---------------------------------------------*/

GLboolean COpenGLWinApp::CreateAppWindow(string sTitle)
{
	hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);

	if(!oglControl.InitOpenGL(hInstance, &hWnd, 3, 1, InitScene, RenderScene, NULL, &oglControl))return false;

	ShowWindow(hWnd, SW_SHOW);

	// Just to send WM_SIZE message again
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	return true;
}

/*-----------------------------------------------

Name:	AppBody

Params:	none

Result:	Main application body infinite loop.

/*---------------------------------------------*/

GLvoid COpenGLWinApp::AppBody()
{
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)break; // If the message was WM_QUIT then exit application
			else
			{
				TranslateMessage(&msg); // Otherwise send message to appropriate window
				DispatchMessage(&msg);
			}
		}
		else if(bAppActive)
		{
			UpdateTimer();
			oglControl.Render(&oglControl);
		}
		else Sleep(200); // Do not consume processor power if application isn't active
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

	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
	semBuf.sem_num = 0;
	semBuf.sem_op  = 1;
	semBuf.sem_flg = SEM_UNDO;
	if (semop(hMutex, &semBuf, 1) != 0)
	{
		cerr<<"RC_UNLOCK_ERROR"<<endl;
		abort();
	}
	semctl(hMutex, 0, IPC_RMID );
}

/*-----------------------------------------------

Name:	MsgHandlerMain

Params:	windows message stuff

Result:	Application messages handler.

/*---------------------------------------------*/

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

GLint WINAPI WinMain(GLvoid* hInstance, GLvoid* hPrevInstance, LPSTR sCmdLine, GLint iShow)
{
	if(!appMain.InitializeApp("01_opengl_3_3"))
		return 0;
	appMain.RegisterAppClass(hInstance);

	if(!appMain.CreateAppWindow("01.) Creating OpenGL 3.3 Window - Tutorial by Michal Bubnar (www.mbsoftworks.sk)"))
		return 0;
	appMain.ResetTimer();

	appMain.AppBody();
	appMain.Shutdown();

	return 0;
}

