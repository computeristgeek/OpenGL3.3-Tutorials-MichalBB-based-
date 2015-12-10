#pragma once

#include "OpenGLControl.h"

class COpenGLWinApp
{
public:
	GLFWwindow* hWnd; // Handle to application window
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

	//long CALLBACK MsgHandlerMain(GLuint hWnd, GLuint uiMsg, GLuint wParam, long lParam);

private:
	GLvoid* hInstance; // Application's instance
	string sAppName;
	void* hMutex;

	GLboolean bAppActive; // To check if application is active (not minimized)
	unsigned long dwLastFrame;
	GLfloat fFrameInterval;
};

extern COpenGLWinApp appMain;

GLvoid InitScene(GLvoid*), RenderScene(GLvoid*);
