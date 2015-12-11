#include "common_header.h"

#include "Lin_OpenGLApp.h"

#include "shaders.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*-----------------------------------------------

Name:	InitScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Initializes OpenGL features that will
		be used.

/*---------------------------------------------*/

float fPyramid[36]; // Pyramid data - 4 triangles of 3 vertices of 3 floats
float fPyramidColor[36]; // Same for color

GLuint uiVBO[2]; // One VBO for vertices positions, one for colors
GLuint uiVAO[1]; // One VAO for pyramid
bool bShowFPS = false;
bool bVerticalSync = true;

CShader shVertex, shFragment;
CShaderProgram spMain;

GLvoid InitScene(GLvoid* lpParam)
{
	// For now, we just clear color to light blue,
	// to see if OpenGL context is working
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Setup pyramid
	// Front face
	fPyramid[0] = 0.0f; fPyramid[1] = 5.0f; fPyramid[2] = 0.0f;
	fPyramid[3] = -3.0f; fPyramid[4] = 0.0f; fPyramid[5] = 3.0f;
	fPyramid[6] = 3.0f; fPyramid[7] = 0.0f; fPyramid[8] = 3.0f;

	// Back face
	fPyramid[9] = 0.0f; fPyramid[10] = 5.0f; fPyramid[11] = 0.0f;
	fPyramid[12] = 3.0f; fPyramid[13] = 0.0f; fPyramid[14] = -3.0f;
	fPyramid[15] = -3.0f; fPyramid[16] = 0.0f; fPyramid[17] = -3.0f;

	// Left face
	fPyramid[18] = 0.0f; fPyramid[19] = 5.0f; fPyramid[20] = 0.0f;
	fPyramid[21] = -3.0f; fPyramid[22] = 0.0f; fPyramid[23] = -3.0f;
	fPyramid[24] = -3.0f; fPyramid[25] = 0.0f; fPyramid[26] = 3.0f;

	// Right face
	fPyramid[27] = 0.0f; fPyramid[28] = 5.0f; fPyramid[29] = 0.0f;
	fPyramid[30] = 3.0f; fPyramid[31] = 0.0f; fPyramid[32] = 3.0f;
	fPyramid[33] = 3.0f; fPyramid[34] = 0.0f; fPyramid[35] = -3.0f;

	// Setup pyramid color

	FOR(i, 4)
	{
		fPyramidColor[i*9] = 1.0f; fPyramidColor[i*9+1] = 0.0f; fPyramidColor[i*9+2] = 0.0f;
		if(i < 2)
		{
			fPyramidColor[i*9+1] = 0.0f; fPyramidColor[i*9+4] = 1.0f; fPyramidColor[i*9+5] = 0.0f;
			fPyramidColor[i*9+2] = 0.0f; fPyramidColor[i*9+7] = 0.0f; fPyramidColor[i*9+8] = 1.0f;
		}
		else
		{
			fPyramidColor[i*9+2] = 0.0f; fPyramidColor[i*9+7] = 1.0f; fPyramidColor[i*9+8] = 0.0f;
			fPyramidColor[i*9+1] = 0.0f; fPyramidColor[i*9+4] = 0.0f; fPyramidColor[i*9+5] = 1.0f;
		}
	}
 
	glGenVertexArrays(1, uiVAO);
	glGenBuffers(2, uiVBO);

	// Setup whole pyramid
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramidColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Load shaders and create shader program

	shVertex.LoadShader("data/shaders/shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("data/shaders/shader.frag", GL_FRAGMENT_SHADER);

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shVertex);
	spMain.AddShaderToProgram(&shFragment);

	spMain.LinkProgram();
	spMain.UseProgram();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

/*-----------------------------------------------

Name:	RenderScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Renders whole scene.

/*---------------------------------------------*/

float fRotationAngle = 0.0f;
const float PIover180 = 3.1415f/180.0f;

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	oglControl->MakeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(uiVAO[0]);

	int iModelViewLoc = glGetUniformLocation(spMain.GetProgramID(), "modelViewMatrix");
	int iProjectionLoc = glGetUniformLocation(spMain.GetProgramID(), "projectionMatrix");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*oglControl->getProjectionMatrix()));

	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 15, 40), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Render rotating pyramid in the middle

	glm::mat4 mCurrent = glm::rotate(mModelView, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// Render translating pyramids

	// One on the left
	mCurrent = glm::translate(mModelView, glm::vec3(-20.0f, 10.0f*float(sin(fRotationAngle*PIover180)), 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// One on the right
	mCurrent = glm::translate(mModelView, glm::vec3(20.0f, -10.0f*float(sin(fRotationAngle*PIover180)), 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// And one translating and rotating on top

	mCurrent = glm::translate(mModelView, glm::vec3(20.0f*float(sin(fRotationAngle*PIover180)), 10.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// And lastly - render scaling pyramid that rotates

	float fScaleValue = 1.5f+float(sin(fRotationAngle*PIover180))*0.5f;
	mCurrent = glm::translate(mModelView, glm::vec3(0.0f, -10.0f, 0.0f));
	mCurrent = glm::scale(mCurrent, glm::vec3(fScaleValue, fScaleValue, fScaleValue));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	fRotationAngle += appMain.sof(120.0f);

	// Add some handlers - with F2, you can toggle FPS showing, with F3 you can toggle V-Sync

	if(glfwGetKey(appMain.hWnd,GLFW_KEY_F2))
	{
		bShowFPS = !bShowFPS;
		if(!bShowFPS)glfwSetWindowTitle(appMain.hWnd, "04.) Going 3D With Transformations - Tutorial by Michal Bubnar (www.mbsoftworks.sk)");
	}
	if(glfwGetKey(appMain.hWnd,GLFW_KEY_F3))
	{
		bVerticalSync = !bVerticalSync;
		oglControl->setVerticalSynchronization(bVerticalSync);
	}
	if(bShowFPS)
	{
		char buf[55]; sprintf(buf, "FPS: %d, V-Sync: %s", oglControl->getFPS(), bVerticalSync ? "On": "Off");
		glfwSetWindowTitle(appMain.hWnd, buf);
	}

	oglControl->SwapBuffersM();
}

/*-----------------------------------------------

Name:	ReleaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
{
	spMain.DeleteProgram();

	glDeleteBuffers(2, uiVBO);
	glDeleteVertexArrays(1, uiVAO);

	shVertex.DeleteShader();
	shFragment.DeleteShader();
}

/*-----------------------------------------------

Name:	key_CB

Params:	[in]	window	The window that received the event.
	[in]	key	The keyboard key that was pressed or released.
	[in]	scancode	The system-specific scancode of the key.
	[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
	[in]	mods	Bit field describing which modifier keys were held down

Result:	Keyboard Callback

/*---------------------------------------------*/

GLvoid key_CB(GLFWwindow* hWnd, int key, int scancode, int action, int mods)
{
	glfwMakeContextCurrent(hWnd);
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			cout<<"Normal Exit:ESC Pressed"<<endl;
			glfwSetWindowShouldClose(hWnd, GL_TRUE);
			break;
		case 'C':
			if(action==GLFW_PRESS && mods==GLFW_MOD_CONTROL)
			{
				cout<<"Normal Exit:^C Pressed"<<endl;
				glfwSetWindowShouldClose(hWnd, GL_TRUE);
			}
			break;
	}
}

/*-----------------------------------------------

Name:	framebuffer_CB

Params:	[in]	window	The window whose framebuffer was resized.
	[in]	width	The new width, in pixels, of the framebuffer.
	[in]	height	The new height, in pixels, of the framebuffer.

Result:	Frame Buffer Size Callback

/*---------------------------------------------*/

GLvoid framebuffer_CB(GLFWwindow* hWnd, int width, int height)
{
	glfwMakeContextCurrent(hWnd);
	appMain.oglControl.ResizeOpenGLViewportFull();
	appMain.oglControl.setProjection3D(45.0f, float(width)/float(height), 0.001f, 1000.0f);
}

/*-----------------------------------------------

Name:	error_CB

Params:	[in]	error	An error code.
	[in]	description	A UTF-8 encoded string describing the error.

Result:	Error Callback

/*---------------------------------------------*/

void error_CB(int error, const char* description)
{
	cerr<<"Error "<<hex<<error<<":"<<description<<endl;
}
