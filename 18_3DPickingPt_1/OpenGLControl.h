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
	GLboolean InitOpenGL(GLvoid* hInstance, GLuint* a_hWnd, GLint iMajorVersion, GLint iMinorVersion, GLvoid (*a_ptrInitScene)(GLvoid*), GLvoid (*a_ptrRenderScene)(GLvoid*), void(*a_ptrReleaseScene)(GLvoid*), GLvoid* lpParam);
	
	GLvoid ResizeOpenGLViewportFull();
	GLvoid SetProjection3D(GLfloat fFOV, GLfloat fAspectRatio, GLfloat fNear, GLfloat fFar);
	GLvoid SetOrtho2D(GLint width, GLint height);

	glm::mat4* GetProjectionMatrix();
	glm::mat4* GetOrthoMatrix();

	GLvoid Render(GLvoid* lpParam);
	GLvoid ReleaseOpenGLControl(GLvoid* lpParam);

	static GLvoid RegisterSimpleOpenGLClass(GLvoid* hInstance);
	static GLvoid UnregisterSimpleOpenGLClass(GLvoid* hInstance);

	GLvoid MakeCurrent();
	GLvoid SwapBuffers();

	GLboolean SetVerticalSynchronization(GLboolean bEnabled);

	GLint GetFPS();

	GLint GetViewportWidth();
	GLint GetViewportHeight();

	COpenGLControl();

private:
	GLboolean InitGLEW(GLvoid* hInstance);

	GLvoid* hDC;
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

	GLvoid (*ptrInitScene)(GLvoid* lpParam), (*ptrRenderScene)(GLvoid* lpParam), (*ptrReleaseScene)(GLvoid* lpParam);
};

long CALLBACK msgHandlerSimpleOpenGLClass(GLuint, GLuint, GLuint, long);