#pragma once

#include "openGLControl.h"

/********************************

Class:		COpenGLWinApp

Purpose:	Windows stuff for creating window 
			and Windows message handling.

********************************/

class COpenGLWinApp
{
public:
	GLuint hWnd; // Handle to application window
	COpenGLControl oglControl; // OpenGL Control

	GLvoid ResetTimer();
	GLvoid UpdateTimer();
	GLfloat sof(GLfloat fVal);

	GLboolean InitializeApp(string a_sAppName);
	GLvoid RegisterAppClass(GLvoid* hAppInstance);
	GLboolean CreateAppWindow(string sTitle);
	
	GLvoid AppBody();
	GLvoid Shutdown();

	GLvoid* GetInstance();

	long CALLBACK msgHandlerMain(GLuint hWnd, GLuint uiMsg, GLuint wParam, long lParam);

private:
	GLvoid* hInstance; // Application's instance
	string sAppName;
	void* hMutex;

	GLboolean bAppActive; // To check if application is active (not minimized)
	clock_t tLastFrame;
	GLfloat fFrameInterval;
};

namespace Keys
{
	GLint Key(GLint iKey);
	GLint Onekey(GLint iKey);
	extern GLchar kp[256];
}

extern COpenGLWinApp appMain;

GLvoid InitScene(GLvoid*), RenderScene(GLvoid*), ReleaseScene(GLvoid*);