#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUMSHADERS 6
#define NUMTEXTURES 3

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
GLuint uiVAOs[1]; // Only one VAO now

CShader shShaders[NUMSHADERS];
CShaderProgram spFogAndLight, spOrtho2D, spFont2D;

CTexture tTextures[NUMTEXTURES];

CFlyingCamera cCamera;

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;

/*-----------------------------------------------

Name:	initScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
			be used.

/*---------------------------------------------*/

#include "static_geometry.h"

GLint iTorusFaces;

GLvoid initScene(GLvoid* lpParam)
{
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
		vCubeTexCoords[i] *= 100.0f;
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

	shShaders[0].loadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shShaders[1].loadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);
	shShaders[2].loadShader("data\\shaders\\ortho2D.vert", GL_VERTEX_SHADER);
	shShaders[3].loadShader("data\\shaders\\ortho2D.frag", GL_FRAGMENT_SHADER);
	shShaders[4].loadShader("data\\shaders\\font2D.frag", GL_FRAGMENT_SHADER);
	shShaders[5].loadShader("data\\shaders\\fog.frag", GL_FRAGMENT_SHADER);

	spFogAndLight.createProgram();
	spFogAndLight.addShaderToProgram(&shShaders[0]);
	spFogAndLight.addShaderToProgram(&shShaders[1]);
	spFogAndLight.addShaderToProgram(&shShaders[5]);
	spFogAndLight.linkProgram();

	spOrtho2D.createProgram();
	spOrtho2D.addShaderToProgram(&shShaders[2]);
	spOrtho2D.addShaderToProgram(&shShaders[3]);
	spOrtho2D.linkProgram();

	spFont2D.createProgram();
	spFont2D.addShaderToProgram(&shShaders[2]);
	spFont2D.addShaderToProgram(&shShaders[4]);
	spFont2D.linkProgram();

	// Load textures

	string sTextureNames[] = {"grass.jpg", "crate.jpg", "metalplate.jpg"};

	FOR(i, NUMTEXTURES) // I know that FOR cycle is useless now, but it was easier to rewrite :)
	{
		tTextures[i].loadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.0f, 0.26f, 0.48f, 1.0f);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.loadSystemFont("arial.ttf", 32);
	ftFont.setShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(-75.0f, 10.0f, 120.0f), glm::vec3(-75.0f, 10.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.setMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.loadSkybox("data\\skyboxes\\jajlands1\\", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
}

/*-----------------------------------------------

Name:	renderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;

#define FOG_EQUATION_LINEAR		0
#define FOG_EQUATION_EXP		1
#define FOG_EQUATION_EXP2		2

namespace FogParameters
{
	GLfloat fDensity = 0.04f;
	GLfloat fStart = 10.0f;
	GLfloat fEnd = 75.0f;
	glm::vec4 vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	GLint iFogEquation = FOG_EQUATION_EXP; // 0 = linear, 1 = exp, 2 = exp2
};

GLvoid renderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	spFogAndLight.useProgram();

	spFogAndLight.setUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
	spFogAndLight.setUniform("sunLight.fAmbientIntensity", 1.0f); // Full light for skybox
	spFogAndLight.setUniform("sunLight.vDirection", glm::vec3(0, -1, 0));

	spFogAndLight.setUniform("matrices.projectionMatrix", oglControl->getProjectionMatrix());
	spFogAndLight.setUniform("gSampler", 0);

	glm::mat4 mModelView = cCamera.look();
	glm::mat4 mModelToCamera;

	spFogAndLight.setUniform("fogParams.iEquation", FogParameters::iFogEquation);
	spFogAndLight.setUniform("fogParams.vFogColor", FogParameters::vFogColor);

	if(FogParameters::iFogEquation == FOG_EQUATION_LINEAR)
	{
		spFogAndLight.setUniform("fogParams.fStart", FogParameters::fStart);
		spFogAndLight.setUniform("fogParams.fEnd", FogParameters::fEnd);
	}
	else
		spFogAndLight.setUniform("fogParams.fDensity", FogParameters::fDensity);

	spFogAndLight.setUniform("matrices.modelViewMatrix", glm::translate(mModelView, cCamera.vEye));
	sbMainSkybox.renderSkybox();

	glBindVertexArray(uiVAOs[0]);
	spFogAndLight.setUniform("sunLight.fAmbientIntensity", 0.55f);
	spFogAndLight.setUniform("matrices.modelViewMatrix", &mModelView);
	
	// Render ground

	tTextures[0].bindTexture();
	glDrawArrays(GL_TRIANGLES, 36, 6);

	// Create a nice spiral from boxes

	tTextures[1].bindTexture();

	GLfloat dx[] = {1.0f, 0.0f, -1.0f, 0.0f};
	GLfloat dz[] = {0.0f, -1.0f, 0.0f, 1.0f};
	GLint iCurDir = 0;
	glm::vec3 vBoxPos(0.0f, 7.48f, 0.0f);
	const GLint iSpiralLevels = 12;
	SFOR(i, 1, iSpiralLevels)
	{
		FOR(j, i)
		{
			mModelToCamera = glm::translate(glm::mat4(1.0), vBoxPos);
			mModelToCamera = glm::scale(mModelToCamera, glm::vec3(15.0f, 15.0f, 15.0f));
			// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
			spFogAndLight.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
			spFogAndLight.setUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			vBoxPos += glm::vec3(15.02f*dx[iCurDir], 0.0f, 15.02f*dz[iCurDir]);
		}
		iCurDir = (iCurDir+1)%4;
	}

	// Render 3 rotated tori to create interesting object

	tTextures[2].bindTexture();
	spFogAndLight.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	// Now it's gonna GLfloat in the air
	glm::vec3 vPos = glm::vec3(0.0f, 50.0, 0.0f);
	mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
	mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	spFogAndLight.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spFogAndLight.setUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);
	glDrawArrays(GL_TRIANGLES, 42, iTorusFaces*3);

	mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
	mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelToCamera = glm::rotate(mModelToCamera, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	spFogAndLight.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spFogAndLight.setUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);
	glDrawArrays(GL_TRIANGLES, 42, iTorusFaces*3);

	mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
	mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle+90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	spFogAndLight.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
	spFogAndLight.setUniform("matrices.modelViewMatrix", mModelView*mModelToCamera);
	glDrawArrays(GL_TRIANGLES, 42, iTorusFaces*3);

	fGlobalAngle += appMain.sof(100.0f);
	cCamera.update();

	// PrGLint something over scene

	spFont2D.useProgram();
	glDisable(GL_DEPTH_TEST);
	GLint w = oglControl->getViewportWidth(), h = oglControl->getViewportHeight();
	spFont2D.setUniform("matrices.projectionMatrix", oglControl->getOrthoMatrix());

	spFont2D.setUniform("vColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	ftFont.printFormatted(20, h-20, 24, "Current fog equation: %s (Press F to change)", FogParameters::iFogEquation == 0 ? "Linear" : (FogParameters::iFogEquation == 1 ? "Exp" : "Exp2"));
	if(FogParameters::iFogEquation == 0)
	{
		ftFont.printFormatted(20, h-50, 24, "Fog start: %.4f (Press + and - to change)", FogParameters::fStart);
		ftFont.printFormatted(20, h-80, 24, "Fog end: %.4f  (Press PGUP and PGDN to change)", FogParameters::fEnd);
		if(Keys::key(VK_ADD))
			FogParameters::fStart += appMain.sof(15.0f);
		if(Keys::key(VK_SUBTRACT))
			FogParameters::fStart -= appMain.sof(15.0f);

		if(Keys::key(VK_PRIOR))
			FogParameters::fEnd += appMain.sof(15.0f);
		if(Keys::key(VK_NEXT))
			FogParameters::fEnd -= appMain.sof(15.0f);
	}
	else
	{
		ftFont.printFormatted(20, h-50, 24, "Fog density: %.4f (Press + and - to change)", FogParameters::fDensity);
		if(Keys::key(VK_ADD))
			FogParameters::fDensity +=	appMain.sof(0.01f);
		if(Keys::key(VK_SUBTRACT))
			FogParameters::fDensity -= appMain.sof(0.01f);
	}

	if(Keys::onekey('F'))FogParameters::iFogEquation = (FogParameters::iFogEquation+1)%3;

	spFont2D.setUniform("vColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	ftFont.print("www.mbsoftworks.sk", 20, 20, 24);

	glEnable(GL_DEPTH_TEST);
	if(Keys::onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->swapBuffers();
}

/*-----------------------------------------------

Name:	releaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid releaseScene(GLvoid* lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].releaseTexture();
	sbMainSkybox.releaseSkybox();

	spFogAndLight.deleteProgram();
	spOrtho2D.deleteProgram();
	spFont2D.deleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].deleteShader();
	ftFont.releaseFont();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.releaseVBO();
}