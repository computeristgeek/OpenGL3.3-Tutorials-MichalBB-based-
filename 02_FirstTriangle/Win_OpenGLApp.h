#pragma once

#include "OpenGLControl.h"

class COpenGLWinApp
{
public:
	HWND hWnd; // Handle to application window
	COpenGLControl oglControl; // OpenGL Control

	GLvoid ResetTimer();
	GLvoid UpdateTimer();
	GLfloat sof(GLfloat fVal);

	GLboolean InitializeApp(string a_sAppName);
	GLvoid RegisterAppClass(HINSTANCE hAppInstance);
	GLboolean CreateAppWindow(string sTitle);

	GLvoid AppBody();
	GLvoid Shutdown();

	HINSTANCE GetInstance();

	LRESULT CALLBACK MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance; // Application's instance
	string sAppName;
	HANDLE hMutex;

	GLboolean bAppActive; // To check if application is active (not minimized)
	DWORD dwLastFrame;
	GLfloat fFrameInterval;
};

namespace Keys
{
	GLint Key(GLint iKey);
	GLint Onekey(GLint iKey);
	extern char kp[256];
}

extern COpenGLWinApp appMain;

GLvoid InitScene(LPVOID), RenderScene(LPVOID);