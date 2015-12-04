#pragma once

#define SIMPLE_OPENGL_CLASS_NAME "simple_openGL_class_name"

/********************************

Class:	COpenGLControl

Purpose: Provides convenient usage
		 of OpenGL.

********************************/

class COpenGLControl
{
public:
	GLboolean InitOpenGL(HINSTANCE hInstance, GLuint* a_hWnd, GLint iMajorVersion, GLint iMinorVersion, GLvoid (*a_ptrInitScene)(LPVOID), GLvoid (*a_ptrRenderScene)(LPVOID), void(*a_ptrReleaseScene)(LPVOID), LPVOID lpParam);
	
	GLvoid ResizeOpenGLViewportFull();
	GLvoid SetProjection3D(GLfloat fFOV, GLfloat fAspectRatio, GLfloat fNear, GLfloat fFar);
	GLvoid SetOrtho2D(GLint width, GLint height);

	glm::mat4* GetProjectionMatrix();
	glm::mat4* GetOrthoMatrix();

	GLvoid Render(LPVOID lpParam);
	GLvoid ReleaseOpenGLControl(LPVOID lpParam);

	static GLvoid RegisterSimpleOpenGLClass(HINSTANCE hInstance);
	static GLvoid UnregisterSimpleOpenGLClass(HINSTANCE hInstance);

	GLvoid MakeCurrent();
	GLvoid SwapBuffers();

	GLboolean SetVerticalSynchronization(GLboolean bEnabled);

	GLint GetFPS();

	GLint GetViewportWidth();
	GLint GetViewportHeight();

	COpenGLControl();

private:
	GLboolean InitGLEW(HINSTANCE hInstance);

	HDC hDC;
	GLuint* hWnd;
	HGLRC hRC;
	static GLboolean bClassRegistered;
	static GLboolean bGlewInitialized;
	GLint iMajorVersion, iMinorVersion;

	// Used for FPS calculation
	GLint iFPSCount, iCurrentFPS;
	clock_t tLastSecond;

	// Matrix for perspective projection
	glm::mat4 mProjection;
	// Matrix for orthographic 2D projection
	glm::mat4 mOrtho;

	// Viewport parameters
	GLint iViewportWidth, iViewportHeight;

	GLvoid (*ptrInitScene)(LPVOID lpParam), (*ptrRenderScene)(LPVOID lpParam), (*ptrReleaseScene)(LPVOID lpParam);
};

LRESULT CALLBACK msgHandlerSimpleOpenGLClass(GLuint, GLuint, WPARAM, LPARAM);