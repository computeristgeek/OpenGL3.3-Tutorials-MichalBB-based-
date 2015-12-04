#pragma once

#include "openGLControl.h"

class COpenGLWinApp
{
public:
	HWND hWnd; // Handle to application window
	COpenGLControl oglControl; // OpenGL Control

	GLvoid resetTimer();
	GLvoid updateTimer();
	GLfloat sof(GLfloat fVal);

	GLboolean initializeApp(string a_sAppName);
	GLvoid registerAppClass(HINSTANCE hAppInstance);
	GLboolean createWindow(string sTitle);
	
	GLvoid appBody();
	GLvoid shutdown();

	HINSTANCE getInstance();

	LRESULT CALLBACK msgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance; // Application's instance
	string sAppName;
	HANDLE hMutex;

	GLboolean bAppActive; // To check if application is active (not minimized)
	clock_t tLastFrame;
	GLfloat fFrameInterval;
};

namespace Keys
{
	GLint key(GLint iKey);
	GLint onekey(GLint iKey);
	extern char kp[256];
}

extern COpenGLWinApp appMain;

GLvoid initScene(LPVOID), renderScene(LPVOID), releaseScene(LPVOID);