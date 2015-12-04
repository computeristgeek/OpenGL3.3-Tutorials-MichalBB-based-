#pragma once

#define SIMPLE_OPENGL_CLASS_NAME "simple_openGL_class_name"

/********************************

Class:	COpenGLControl

Purpose:	Provides convenient usage
			of OpenGL

********************************/

class COpenGLControl
{
public:
	GLboolean initOpenGL(HINSTANCE hInstance, GLuint* a_hWnd, GLint iMajorVersion, GLint iMinorVersion, GLvoid (*a_initScene)(GLvoid*), GLvoid (*a_renderScene)(GLvoid*), void(*a_releaseScene)(GLvoid*), GLvoid* lpParam);
	
	GLvoid resizeOpenGLViewportFull();
	GLvoid setProjection3D(GLfloat fFOV, GLfloat fAspectRatio, GLfloat fNear, GLfloat fFar);
	GLvoid setOrtho2D(GLint width, GLint height);

	glm::mat4* getProjectionMatrix();
	glm::mat4* getOrthoMatrix();

	GLvoid render(GLvoid* lpParam);
	GLvoid releaseOpenGLControl(GLvoid* lpParam);

	static GLvoid registerSimpleOpenGLClass(HINSTANCE hInstance);
	static GLvoid unregisterSimpleOpenGLClass(HINSTANCE hInstance);

	GLvoid makeCurrent();
	GLvoid swapBuffers();

	GLboolean setVerticalSynchronization(GLboolean bEnabled);

	GLint getFPS();

	GLint getViewportWidth();
	GLint getViewportHeight();

	COpenGLControl();

private:
	GLboolean initGLEW(HINSTANCE hInstance);

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

	GLvoid (*initScene)(GLvoid* lpParam), (*renderScene)(GLvoid* lpParam), (*releaseScene)(GLvoid* lpParam);
};

LRESULT CALLBACK msgHandlerSimpleOpenGLClass(GLuint, GLuint, WPARAM, LPARAM);