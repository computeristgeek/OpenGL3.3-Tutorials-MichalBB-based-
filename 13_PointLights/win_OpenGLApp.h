#pragma once

#include "openGLControl.h"

class COpenGLWinApp
{
public:
	GLuint hWnd; // Handle to application window
	COpenGLControl oglControl; // OpenGL Control

	GLvoid resetTimer();
	GLvoid updateTimer();
	GLfloat sof(GLfloat fVal);

	GLboolean initializeApp(string a_sAppName);
	GLvoid registerAppClass(GLvoid* hAppInstance);
	GLboolean createWindow(string sTitle);
	
	GLvoid appBody();
	GLvoid shutdown();

	GLvoid* getInstance();

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
	GLint key(GLint iKey);
	GLint onekey(GLint iKey);
	extern GLchar kp[256];
}

extern COpenGLWinApp appMain;

GLvoid initScene(GLvoid*), renderScene(GLvoid*), releaseScene(GLvoid*);