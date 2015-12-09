#pragma once

#include "OpenGLControl.h"
/*----------------Linux Semaphores---------*/
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*----------------Linux Semaphores---------*/

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

	long CALLBACK MsgHandlerMain(GLuint hWnd, GLuint uiMsg, GLuint wParam, long lParam);

private:
	GLvoid* hInstance; // Application's instance
	string sAppName;
	int hMutex;
	struct sembuf   semBuf;

	GLboolean bAppActive; // To check if application is active (not minimized)
	unsigned long dwLastFrame;
	GLfloat fFrameInterval;
};

namespace Keys
{
	GLint Key(GLint iKey);
	GLint Onekey(GLint iKey);
	extern GLchar kp[256];
}

extern COpenGLWinApp appMain;

GLvoid InitScene(GLvoid*), RenderScene(GLvoid*);