#include "common_header.h"

#include "Lin_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
GLuint uiVAOs[1]; // Only one VAO now

CShader shShaders[5];
CShaderProgram spDirectionalLight, spOrtho2D, spFont2D;

#define NUMTEXTURES 3

CTexture tTextures[NUMTEXTURES];

CFlyingCamera cCamera;

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;

/*-----------------------------------------------

Name:	InitScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Initializes OpenGL features that will
		be used.

/*---------------------------------------------*/

#include "static_geometry.h"

GLint iTorusFaces;

GLvoid InitScene(GLvoid* lpParam)
{
	// For now, we just clear color to light blue,
	// to see if OpenGL context is working
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	vboSceneObjects.createVBO();
	glGenVertexArrays(1, uiVAOs); // Create one VAO
	glBindVertexArray(uiVAOs[0]);

	vboSceneObjects.bindVBO();

	// Add cube to VBO

	FOR(i, 36)
	{
		vboSceneObjects.addData(&vCubeVertices[i], sizeof(glm::vec3));
		vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
		vboSceneObjects.addData(&vCubeNormals[i/6], sizeof(glm::vec3));
	}

	// Add ground to VBO

	FOR(i, 6)
	{
		vboSceneObjects.addData(&vGround[i], sizeof(glm::vec3));
		vCubeTexCoords[i] *= 10.0f;
		vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
		glm::vec3 vGroundNormal(0.0f, 1.0f, 0.0f);
		vboSceneObjects.addData(&vGroundNormal, sizeof(glm::vec3));
	}

	iTorusFaces = generateTorus(vboSceneObjects, 7.0f, 2.0f, 20, 20);
	vboSceneObjects.uploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	// Load shaders and create shader programs

	shShaders[0].LoadShader("data/shaders/shader.vert", GL_VERTEX_SHADER);
	shShaders[1].LoadShader("data/shaders/shader.frag", GL_FRAGMENT_SHADER);
	shShaders[2].LoadShader("data/shaders/ortho2D.vert", GL_VERTEX_SHADER);
	shShaders[3].LoadShader("data/shaders/ortho2D.frag", GL_FRAGMENT_SHADER);
	shShaders[4].LoadShader("data/shaders/font2D.frag", GL_FRAGMENT_SHADER);
	
	spDirectionalLight.CreateProgram();
	spDirectionalLight.AddShaderToProgram(&shShaders[0]);
	spDirectionalLight.AddShaderToProgram(&shShaders[1]);
	spDirectionalLight.LinkProgram();

	spOrtho2D.CreateProgram();
	spOrtho2D.AddShaderToProgram(&shShaders[2]);
	spOrtho2D.AddShaderToProgram(&shShaders[3]);
	spOrtho2D.LinkProgram();

	spFont2D.CreateProgram();
	spFont2D.AddShaderToProgram(&shShaders[2]);
	spFont2D.AddShaderToProgram(&shShaders[4]);
	spFont2D.LinkProgram();

	// Load textures

	string sTextureNames[] = {"grass.jpg", "crate.jpg", "metalplate.jpg"};

	FOR(i, NUMTEXTURES) // I know that FOR cycle is useless now, but it was easier to rewrite :)
	{
		tTextures[i].loadTexture2D("data/textures/"+sTextureNames[i], true);
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.0f, 0.26f, 0.48f, 1.0f);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.loadSystemFont("arial.ttf", 32);
	ftFont.setShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 155.0f, 0.1f);
	//cCamera.setMovingKeys('W', 'S', 'A', 'D'); moved to key_CB

	sbMainSkybox.loadSkybox("data/skyboxes/jajlands1/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
}

/*-----------------------------------------------

Name:	RenderScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	oglControl->MakeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	spDirectionalLight.UseProgram();

	spDirectionalLight.setUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
	spDirectionalLight.setUniform("sunLight.fAmbientIntensity", 1.0f); // Full light for skybox
	spDirectionalLight.setUniform("sunLight.vDirection", glm::vec3(0, -1, 0));

	spDirectionalLight.setUniform("projectionMatrix", oglControl->getProjectionMatrix());
	spDirectionalLight.setUniform("gSampler", 0);

	glm::mat4 mModelView = cCamera.look();
	glm::mat4 mModelToCamera;

	spDirectionalLight.setUniform("modelViewMatrix", glm::translate(mModelView, cCamera.vEye));
	sbMainSkybox.renderSkybox();

	glBindVertexArray(uiVAOs[0]);
	spDirectionalLight.setUniform("sunLight.fAmbientIntensity", 0.55f);
	spDirectionalLight.setUniform("modelViewMatrix", &mModelView);
	
	// Render ground

	tTextures[0].bindTexture();
	glDrawArrays(GL_TRIANGLES, 36, 6);

	tTextures[1].bindTexture();

	SFOR(nb, 1, 9)
	{
		GLint iCnt = nb > 5 ? 10-nb : nb;
		FOR(i, iCnt)
		{
			glm::vec3 vPos = glm::vec3(-20.0f+nb*8.02f, -6.0f+i*8.02f, -50.0f);
			mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
			mModelToCamera = glm::scale(mModelToCamera, glm::vec3(8.0f, 8.0f, 8.0f));
			// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
			spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
			spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	// Render 3 rotated tori to create interesting object

	tTextures[2].bindTexture();
	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	glm::vec3 vPos = glm::vec3(0.0f, 0.0, 40.0f);
	mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
	mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
	glDrawArrays(GL_TRIANGLES, 42, iTorusFaces*3);

	mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
	mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelToCamera = glm::rotate(mModelToCamera, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
	glDrawArrays(GL_TRIANGLES, 42, iTorusFaces*3);

	mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
	mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle+90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
	glDrawArrays(GL_TRIANGLES, 42, iTorusFaces*3);

	fGlobalAngle += appMain.sof(100.0f);

	// PrGLint something over scene

	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.setUniform("projectionMatrix", oglControl->getOrthoMatrix());
	spFont2D.setUniform("vColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	ftFont.print("www.mbsoftworks.sk", 20, 20);

	glEnable(GL_DEPTH_TEST);

	oglControl->SwapBuffersM();
}

/*-----------------------------------------------

Name:	ReleaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].releaseTexture();
	sbMainSkybox.releaseSkybox();

	spDirectionalLight.DeleteProgram();
	spOrtho2D.DeleteProgram();
	spFont2D.DeleteProgram();
	FOR(i, 4)shShaders[i].DeleteShader();
	ftFont.releaseFont();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.releaseVBO();
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
		case 'W': case 'S': case 'A': case 'D':
			// Get view direction
			glm::vec3 vMove = cCamera.vView-cCamera.vEye;
			vMove = glm::normalize(vMove);
			vMove *= cCamera.fSpeed;

			glm::vec3 vStrafe = glm::cross(cCamera.vView-cCamera.vEye, cCamera.vUp);
			vStrafe = glm::normalize(vStrafe);
			vStrafe *= cCamera.fSpeed;

			GLint iMove = 0;
			glm::vec3 vMoveBy;
			// Get vector of move
			if(key=='W')vMoveBy += vMove*appMain.sof(1.0f);
			if(key=='S')vMoveBy -= vMove*appMain.sof(1.0f);
			if(key=='A')vMoveBy -= vStrafe*appMain.sof(1.0f);
			if(key=='D')vMoveBy += vStrafe*appMain.sof(1.0f);
			cCamera.vEye += vMoveBy; cCamera.vView += vMoveBy;
			break;
	}
}

/*-----------------------------------------------

Name:	mousepos_CB

Params:	[in]	hWnd	The window that received the event.
	[in]	xpos	The new x-coordinate, in screen coordinates, of the cursor.
	[in]	ypos	The new y-coordinate, in screen coordinates, of the cursor.

Result:	Mouse Position Callback

/*---------------------------------------------*/

GLvoid mousepos_CB(GLFWwindow* hWnd, double xpos, double ypos)
{
	cCamera.update();
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
	appMain.oglControl.setOrtho2D(width, height);
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
