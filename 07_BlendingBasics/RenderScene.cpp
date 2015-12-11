#include "common_header.h"

#include "Lin_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "walkingCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position and 2 floats for texture coordinate */

CVertexBufferObject vboSceneObjects;
GLuint uiVAOs[2]; // First VAO is for textured objects, second for only colored (we must disable texture coordinates attribute)

CShader shVertexTex, shFragmentTex, shVertexCol, shFragmentCol;
CShaderProgram spTextured, spColored;

CTexture tBlueIce, tBox;

CWalkingCamera cCamera;

/*---------------------------------------------*/

/*-----------------------------------------------

Name:	InitScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Initializes OpenGL features that will
		be used.

/*---------------------------------------------*/

#include "static_geometry.h"

GLvoid InitScene(GLvoid* lpParam)
{
	// For now, we just clear color to light blue,
	// to see if OpenGL context is working
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	vboSceneObjects.createVBO();
	glGenVertexArrays(2, uiVAOs); // Create one VAO
	glBindVertexArray(uiVAOs[0]);

	vboSceneObjects.bindVBO();

	// Add cube to VBO

	FOR(i, 36)
	{
		vboSceneObjects.addData(&vCubeVertices[i], sizeof(glm::vec3));
		vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
	}

	// Add ground to VBO

	FOR(i, 6)
	{
		vboSceneObjects.addData(&vGround[i], sizeof(glm::vec3));
		vCubeTexCoords[i] *= 10.0f;
		vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
	}

	vboSceneObjects.uploadDataToGPU(GL_STATIC_DRAW);
	
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));

	// Setup another VAO for untextured objects

	glBindVertexArray(uiVAOs[1]);
	// Here enable only first vertex attribute - position (no texture coordinates)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), 0);

	// Load shaders and create shader programs

	shVertexTex.LoadShader("data/shaders/shaderTex.vert", GL_VERTEX_SHADER);
	shFragmentTex.LoadShader("data/shaders/shaderTex.frag", GL_FRAGMENT_SHADER);
	shVertexCol.LoadShader("data/shaders/shaderCol.vert", GL_VERTEX_SHADER);
	shFragmentCol.LoadShader("data/shaders/shaderCol.frag", GL_FRAGMENT_SHADER);

	spTextured.CreateProgram();
	spTextured.AddShaderToProgram(&shVertexTex);
	spTextured.AddShaderToProgram(&shFragmentTex);
	spTextured.LinkProgram();

	spColored.CreateProgram();
	spColored.AddShaderToProgram(&shVertexCol);
	spColored.AddShaderToProgram(&shFragmentCol);
	spColored.LinkProgram();

	tBlueIce.loadTexture2D("data/textures/blueice.jpg", true);
	tBlueIce.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	tBox.loadTexture2D("data/textures/box.jpg", true);
	tBox.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.2f, 0.32f, 0.5f, 1.0f);
	
	cCamera = CWalkingCamera(glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, -19.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);
}

/*-----------------------------------------------

Name:	RenderScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;

glm::vec4 vBoxColors[] = 
{
	glm::vec4(1.0f, 0.0f, 0.0f, 0.5f), // Red
	glm::vec4(0.0f, 1.0f, 0.0f, 0.1f), // Green
	glm::vec4(0.0f, 0.0f, 1.0f, 0.25f), // Blue
	glm::vec4(0.25f, 0.0f, 0.5f, 0.3f), // Dark Purple
	glm::vec4(1.0f, 0.5f, 0.0f, 0.47f), // Orange
};

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	oglControl->MakeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// First render textured objects

	glEnable(GL_TEXTURE_2D);
	spTextured.UseProgram();
	glBindVertexArray(uiVAOs[0]);

	GLint iModelViewLoc = glGetUniformLocation(spTextured.GetProgramID(), "modelViewMatrix");
	GLint iProjectionLoc = glGetUniformLocation(spTextured.GetProgramID(), "projectionMatrix");
	GLint iColorLoc = glGetUniformLocation(spTextured.GetProgramID(), "color");

	glm::vec4 vWhiteColor(1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4fv(iColorLoc, 1, glm::value_ptr(vWhiteColor)); // Set white for textures

	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*oglControl->getProjectionMatrix()));

	glm::mat4 mModelView = cCamera.look();
	glm::mat4 mCurrent = mModelView;

	// Render ground

	tBlueIce.bindTexture();
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mModelView));
	glDrawArrays(GL_TRIANGLES, 36, 6);

	// Render 5 opaque boxes

	tBox.bindTexture();

	FOR(i, 5)
	{
		GLfloat fSign = -1.0f+float(i%2)*2.0f; // This just returns -1.0f or 1.0f (try to examine this)
		glm::vec3 vPos = glm::vec3(fSign*15.0f, 0.0f, 50.0f-float(i)*25.0f);
		mCurrent = glm::translate(mModelView, vPos);
		mCurrent = glm::scale(mCurrent, glm::vec3(8.0f, 8.0f, 8.0f));
		mCurrent = glm::rotate(mCurrent, fGlobalAngle+i*50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Now switch to only colored rendering

	glDisable(GL_TEXTURE_2D);
	spColored.UseProgram();
	glBindVertexArray(uiVAOs[1]);

	iModelViewLoc = glGetUniformLocation(spColored.GetProgramID(), "modelViewMatrix");
	iProjectionLoc = glGetUniformLocation(spColored.GetProgramID(), "projectionMatrix");
	iColorLoc = glGetUniformLocation(spColored.GetProgramID(), "color");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*oglControl->getProjectionMatrix()));

	// Render 5 transparent boxes

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0); // Disable writing to depth buffer

	FOR(i, 5)
	{
		GLfloat fSign = 1.0f-float(i%2)*2.0f; // Same case as before -  -1.0f or 1.0f
		glm::vec3 vPos = glm::vec3(fSign*15.0f, 0.0f, 50.0f-float(i)*25.0f);
		mCurrent = glm::translate(mModelView, vPos);
		mCurrent = glm::scale(mCurrent, glm::vec3(8.0f, 8.0f, 8.0f));
		mCurrent = glm::rotate(mCurrent, fGlobalAngle*0.8f+i*30.0f, glm::vec3(1.0f, 0.0f, 0.0f)); // Just a variation of first rotating
		glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
		glUniform4fv(iColorLoc, 1, glm::value_ptr(vBoxColors[i]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glDisable(GL_BLEND);
	glDepthMask(1); // Re-enable writing to depth buffer

	fGlobalAngle += appMain.sof(100.0f);
	cCamera.update();

	oglControl->SwapBuffersM();
}

/*-----------------------------------------------

Name:	ReleaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
{
	spTextured.DeleteProgram();
	spColored.DeleteProgram();

	shFragmentTex.DeleteShader();
	shFragmentCol.DeleteShader();
	shVertexTex.DeleteShader();
	shVertexCol.DeleteShader();

	glDeleteVertexArrays(2, uiVAOs);
	vboSceneObjects.releaseVBO();
	tBox.releaseTexture();
	tBlueIce.releaseTexture();
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
		case 'W':
			cCamera.move(appMain.sof(cCamera.getfSpeed()));
			break;
		case 'S':
			cCamera.move(appMain.sof(-cCamera.getfSpeed()));
			break;
		case 'A':
			cCamera.rotateViewY(appMain.sof(90.0f));
			break;
		case 'D':
			cCamera.rotateViewY(appMain.sof(-90.0f));
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
