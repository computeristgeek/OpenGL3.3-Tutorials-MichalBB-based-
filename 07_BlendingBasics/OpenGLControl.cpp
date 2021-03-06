#include "common_header.h"

#include "OpenGLControl.h"
#include <glm/gtc/matrix_transform.hpp>

GLboolean COpenGLControl::bClassRegistered = false, COpenGLControl::bGlewInitialized = false;

COpenGLControl::COpenGLControl()
{
	iFPSCount = 0;
	iCurrentFPS = 0;
}

/*-----------------------------------------------

Name:	InitGLEW

Params:	none

Result:	Creates fake window and OpenGL rendering
		context, within which GLEW is initialized.

/*---------------------------------------------*/

GLboolean COpenGLControl::InitGLEW(GLvoid* hInstance)
{
	if(bGlewInitialized)return true;

	RegisterSimpleOpenGLClass(hInstance);

	glfwMakeContextCurrent(hWnd);

	GLboolean bResult = true;

	glewExperimental=true;
	if(!bGlewInitialized)
	{
		GLint err= glewInit();
		if(err != GLEW_OK)
		{
			MessageBox(*hWnd, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
			bResult = false;

		}
		bGlewInitialized = true;
	}
	if(!bResult) glfwDestroyWindow(hWnd);

	return bResult;
}

/*-----------------------------------------------

Name:	InitOpenGL

Params:	hInstance - application instance
			a_hWnd - window to init OpenGL into
			a_iMajorVersion - Major version of OpenGL
			a_iMinorVersion - Minor version of OpenGL
			a_initScene - pointer to init function
			a_renderScene - pointer to render function
			a_releaseScene - optional parameter of release
			function

Result:	Initializes OpenGL rendering context
			of specified version. If succeeds,
			returns true.

/*---------------------------------------------*/

GLboolean COpenGLControl::InitOpenGL(GLvoid* hInstance, GLFWwindow* a_hWnd, GLint iMajorVersion, GLint iMinorVersion, GLvoid (*a_InitScene)(GLvoid*), GLvoid (*a_RenderScene)(GLvoid*), void(*a_ReleaseScene)(GLvoid*), GLvoid* lpParam)
{	

	hWnd = a_hWnd;

	GLboolean bError = false;

	if(iMajorVersion <= 2 && iMinorVersion <= 2)
	{
		bError = true;
	}
	else 
	{
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, iMajorVersion); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, iMinorVersion);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 32);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		if(hWnd==NULL)
		{
			MessageBox(*hWnd, "Failed to Create Window: \
					Double Buffer:OpenGL3.3:32c:32d:800x600", "Fatal Error", MB_ICONERROR);
			return false;
		}

		if(!InitGLEW(hInstance))return false;
	}
	
	if(bError)
	{
		// Generate error messages
		GLchar sErrorMessage[255], sErrorTitle[255];
		sprintf(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!", iMajorVersion, iMinorVersion);
		sprintf(sErrorTitle, "OpenGL %d.%d Not Supported", iMajorVersion, iMinorVersion);
		MessageBox(*hWnd, sErrorMessage, sErrorTitle, MB_ICONINFORMATION);
		return false;
	}

	RenderScene = a_RenderScene;
	InitScene = a_InitScene;
	ReleaseScene = a_ReleaseScene;

	if(InitScene != NULL)InitScene(lpParam);

	return true;
}

/*-----------------------------------------------

Name:	ResizeOpenGLViewportFull

Params:	none

Result:	Resizes viewport to full window with perspective
		projection.

/*---------------------------------------------*/

GLvoid COpenGLControl::ResizeOpenGLViewportFull()
{
	if(hWnd == NULL)return;
	GLint width, height;
	glfwGetFramebufferSize(hWnd,&width,&height);
	glViewport(0, 0, width, height);
}

/*-----------------------------------------------

Name:	setProjection3D

Params:	fFOV - field of view angle
		fAspectRatio - aspect ration of width/height
		fNear, fFar - distance of near and far clipping plane

Result:	Calculates projection matrix.

/*---------------------------------------------*/

void COpenGLControl::setProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar)
{
	mProjection = glm::perspective(fFOV, fAspectRatio, fNear, fFar);
}

/*-----------------------------------------------

Name:	getProjectionMatrix()

Params:	none

Result:	Retrieves pointer to projection matrix

/*---------------------------------------------*/

glm::mat4* COpenGLControl::getProjectionMatrix()
{
	return &mProjection;
}

/*-----------------------------------------------

Name:	RegisterSimpleOpenGLClass

Params:	hInstance - application instance

Result:	Registers simple OpenGL window class.

/*---------------------------------------------*/

GLvoid COpenGLControl::RegisterSimpleOpenGLClass(GLvoid* hInstance)
{
	if(bClassRegistered)return;
	bClassRegistered = true;
}

/*-----------------------------------------------

Name:	UnregisterSimpleOpenGLClass

Params:	hInstance - application instance

Result:	Unregisters simple OpenGL window class.

/*---------------------------------------------*/

GLvoid COpenGLControl::UnregisterSimpleOpenGLClass(GLvoid* hInstance)
{
	if(bClassRegistered)
	{
		bClassRegistered = false;
	}
}

/*-----------------------------------------------

Name:	SwapBuffersM

Params:	none

Result:	Swaps back and front buffer.

/*---------------------------------------------*/

GLvoid COpenGLControl::SwapBuffersM()
{
	glfwSwapBuffers(hWnd);
}

/*-----------------------------------------------

Name:	MakeCurrent

Params:	none

Result:	Makes current device and OpenGL rendering
			context to those associated with OpenGL
			control.

/*---------------------------------------------*/

GLvoid COpenGLControl::MakeCurrent()
{
	glfwMakeContextCurrent(hWnd);
}

/*-----------------------------------------------

Name:	Render

Params:	lpParam - pointer to whatever you want

Result:	Calls previously set render function.

/*---------------------------------------------*/

GLvoid COpenGLControl::Render(GLvoid* lpParam)
{
	clock_t tCurrent = clock();
	if( (tCurrent-tLastSecond) >= CLOCKS_PER_SEC)
	{
		tLastSecond += CLOCKS_PER_SEC;
		iFPSCount = iCurrentFPS;
		iCurrentFPS = 0;
	}
	if(RenderScene)RenderScene(lpParam);
	iCurrentFPS++;
}

/*-----------------------------------------------

Name:	getFPS

Params:	none

Result:	Returns FPS count in last second.

/*---------------------------------------------*/

int COpenGLControl::getFPS()
{
	return iFPSCount;
}

/*-----------------------------------------------

Name:	ReleaseOpenGLControl

Params:	lpParam - pointer to whatever you want

Result:	Calls previously set release function
		and deletes rendering context.

/*---------------------------------------------*/

GLvoid COpenGLControl::ReleaseOpenGLControl(GLvoid* lpParam)
{
	if(ReleaseScene)ReleaseScene(lpParam);
}

/*-----------------------------------------------

Name:	setVerticalSynchronization

Params: bEnabled - whether to enable V-Sync

Result:	Guess what it does :)

/*---------------------------------------------*/

bool COpenGLControl::setVerticalSynchronization(bool bEnabled)
{
	if(!glfwSwapInterval)return false;
	if(bEnabled)glfwSwapInterval(1);
	else glfwSwapInterval(0);
	return true;
 }
