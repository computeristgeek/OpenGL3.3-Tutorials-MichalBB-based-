#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "walkingCamera.h"

#include "freeTypeFont.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
UINT uiVAOs[1]; // Only one VAO now

CShader shShaders[5];
CShaderProgram spDirectionalLight, spOrtho2D, spFont2D;

#define NUMTEXTURES 1

CTexture tTextures[NUMTEXTURES];

CWalkingCamera cCamera;

CFreeTypeFont ftFont;

/*-----------------------------------------------

Name:		initScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
			be used.

/*---------------------------------------------*/

#include "static_geometry.h"

GLint iTorusFaces;

GLvoid initScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	vboSceneObjects.createVBO();
	glGenVertexArrays(1, uiVAOs); // Create one VAO
	glBindVertexArray(uiVAOs[0]);

	vboSceneObjects.bindVBO();
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

	shShaders[0].loadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shShaders[1].loadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);
	shShaders[2].loadShader("data\\shaders\\ortho2D.vert", GL_VERTEX_SHADER);
	shShaders[3].loadShader("data\\shaders\\ortho2D.frag", GL_FRAGMENT_SHADER);
	shShaders[4].loadShader("data\\shaders\\font2D.frag", GL_FRAGMENT_SHADER);

	spDirectionalLight.createProgram();
	spDirectionalLight.addShaderToProgram(&shShaders[0]);
	spDirectionalLight.addShaderToProgram(&shShaders[1]);
	spDirectionalLight.linkProgram();

	spOrtho2D.createProgram();
	spOrtho2D.addShaderToProgram(&shShaders[2]);
	spOrtho2D.addShaderToProgram(&shShaders[3]);
	spOrtho2D.linkProgram();

	spFont2D.createProgram();
	spFont2D.addShaderToProgram(&shShaders[2]);
	spFont2D.addShaderToProgram(&shShaders[4]);
	spFont2D.linkProgram();

	// Load textures

	string sTextureNames[] = {"rust.jpg"};

	FOR(i, NUMTEXTURES) // I know that FOR cycle is useless now, but it was easier to rewrite :)
	{
		tTextures[i].loadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.loadSystemFont("comic.ttf", 32);
	ftFont.setShaderProgram(&spFont2D);
	
	cCamera = CWalkingCamera(glm::vec3(0.0f, 1.0f, -30.0f), glm::vec3(0.0f, 1.0f, -29.0f), glm::vec3(0.0f, 1.0f, 0.0f), 35.0f);
}

/*-----------------------------------------------

Name:	renderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;

GLint iFontSize = 24; // This is default printed font size

GLvoid renderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	spDirectionalLight.useProgram();
	glBindVertexArray(uiVAOs[0]);

	// Set some lighting parameters

	spDirectionalLight.setUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
	spDirectionalLight.setUniform("sunLight.fAmbientIntensity", 0.25f);
	spDirectionalLight.setUniform("sunLight.vDirection", glm::vec3(0, -1, 0));

	spDirectionalLight.setUniform("projectionMatrix", oglControl->getProjectionMatrix());

	glm::mat4 mModelView = cCamera.look();
	glm::mat4 mModelToCamera = glm::rotate(glm::mat4(1.0f), fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	
	// Render torus

	spDirectionalLight.setUniform("gSampler", 0);
	spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	tTextures[0].bindTexture();
	glDrawArrays(GL_TRIANGLES, 0, iTorusFaces*3);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	
	spFont2D.useProgram();
	// Font color, you can even change transparency of font with alpha parameter
	spFont2D.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spFont2D.setUniform("projectionMatrix", oglControl->getOrthoMatrix());

	char buf[255];
	sprintf(buf, "Font Size: %d\nPress UP and DOWN arrow key to change\n\nTotal Torus Faces: %d", iFontSize, iTorusFaces);

	ftFont.print(buf, 20, oglControl->getViewportHeight()-10-iFontSize, iFontSize);

	ftFont.print("www.mbsoftworks.sk", 20, 20, 24);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	fGlobalAngle += appMain.sof(100.0f);
	cCamera.update();

	if(Keys::onekey(VK_UP))iFontSize++;
	if(Keys::onekey(VK_DOWN))iFontSize--;
	if(Keys::onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->swapBuffers();
}

/*-----------------------------------------------

Name:	releaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid releaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].releaseTexture();

	spDirectionalLight.deleteProgram();
	spOrtho2D.deleteProgram();
	spFont2D.deleteProgram();
	FOR(i, 5)shShaders[i].deleteShader();
	ftFont.releaseFont();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.releaseVBO();
}