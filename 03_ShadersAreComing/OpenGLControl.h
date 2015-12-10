#pragma once

#define SIMPLE_OPENGL_CLASS_NAME "Simple_openGL_class"

/********************************

Class:	COpenGLControl

Purpose:	Provides convenient usage
			of OpenGL

********************************/

#include <GLFW/glfw3.h>

class COpenGLControl
{
public:
	GLboolean InitOpenGL(GLvoid* hInstance, GLFWwindow* a_hWnd, GLint iMajorVersion, GLint iMinorVersion, GLvoid (*a_InitScene)(GLvoid*), GLvoid (*a_RenderScene)(GLvoid*), void(*a_ReleaseScene)(GLvoid*), GLvoid* lpParam);
	
	GLvoid ResizeOpenGLViewportFull();

	GLvoid Render(GLvoid* lpParam);
	GLvoid ReleaseOpenGLControl(GLvoid* lpParam);

	static GLvoid RegisterSimpleOpenGLClass(GLvoid* hInstance);
	static GLvoid UnregisterSimpleOpenGLClass(GLvoid* hInstance);

	GLvoid MakeCurrent();
	GLvoid SwapBuffersM();

private:
	GLboolean InitGLEW(GLvoid* hInstance);

	GLvoid* hDC;
	GLFWwindow* hWnd;
	static GLboolean bClassRegistered;
	static GLboolean bGlewInitialized;
	GLint iMajorVersion, iMinorVersion;

	GLvoid (*InitScene)(GLvoid* lpParam), (*RenderScene)(GLvoid* lpParam), (*ReleaseScene)(GLvoid* lpParam);
};
