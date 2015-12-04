#pragma once

#define SIMPLE_OPENGL_CLASS_NAME "Simple_openGL_class"

/********************************

Class:	COpenGLControl

Purpose:	Provides convenient usage
			of OpenGL

********************************/

class COpenGLControl
{
public:
	GLboolean InitOpenGL(HINSTANCE hInstance, GLuint* a_hWnd, GLint iMajorVersion, GLint iMinorVersion, GLvoid (*a_InitScene)(LPVOID), GLvoid (*a_RenderScene)(LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam);
	
	GLvoid ResizeOpenGLViewportFull();

	GLvoid Render(LPVOID lpParam);
	GLvoid ReleaseOpenGLControl(LPVOID lpParam);

	static GLvoid RegisterSimpleOpenGLClass(HINSTANCE hInstance);
	static GLvoid UnregisterSimpleOpenGLClass(HINSTANCE hInstance);

	GLvoid MakeCurrent();
	GLvoid SwapBuffersM();

private:
	GLboolean InitGLEW(HINSTANCE hInstance);

	HDC hDC;
	GLuint* hWnd;
	HGLRC hRC;
	static GLboolean bClassRegistered;
	static GLboolean bGlewInitialized;
	GLint iMajorVersion, iMinorVersion;

	GLvoid (*InitScene)(LPVOID lpParam), (*RenderScene)(LPVOID lpParam), (*ReleaseScene)(LPVOID lpParam);
};

LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(GLuint, GLuint, WPARAM, LPARAM);