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

#define NUMSHADERS 10
#define NUMTEXTURES 4

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;

GLuint uiVAOs[1]; // Only one VAO now

CShader shShaders[NUMSHADERS];
CShaderProgram spPointLights, spOrtho2D, spFont2D, spColor;

CTexture tTextures[NUMTEXTURES];

CFlyingCamera cCamera;

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;

/*-----------------------------------------------

Name:    initScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
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
		glm::vec2 vCoord = vCubeTexCoords[i] * 100.0f;
		vboSceneObjects.addData(&vCoord, sizeof(glm::vec2));
		glm::vec3 vGroundNormal(0.0f, 1.0f, 0.0f);
		vboSceneObjects.addData(&vGroundNormal, sizeof(glm::vec3));
	}

	// Add building to VBO

	FOR(i, 24)
	{
		vboSceneObjects.addData(&vBuilding[i], sizeof(glm::vec3));
		glm::vec2 vCoord = vCubeTexCoords[i%6]*10.0f;
		vboSceneObjects.addData(&vCoord, sizeof(glm::vec2));
		vboSceneObjects.addData(&vBuildingNormals[i/6], sizeof(glm::vec3));
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
	shShaders[5].loadShader("data\\shaders\\directionalLight.frag", GL_FRAGMENT_SHADER);
	shShaders[6].loadShader("data\\shaders\\pointLight.frag", GL_FRAGMENT_SHADER);
	shShaders[7].loadShader("data\\shaders\\fog.frag", GL_FRAGMENT_SHADER);

	shShaders[8].loadShader("data\\shaders\\color.vert", GL_VERTEX_SHADER);
	shShaders[9].loadShader("data\\shaders\\color.frag", GL_FRAGMENT_SHADER);

	spPointLights.createProgram();
	spPointLights.addShaderToProgram(&shShaders[0]);
	spPointLights.addShaderToProgram(&shShaders[1]); 
	spPointLights.addShaderToProgram(&shShaders[5]);
	spPointLights.addShaderToProgram(&shShaders[6]);
	spPointLights.linkProgram();

	spOrtho2D.createProgram();
	spOrtho2D.addShaderToProgram(&shShaders[2]);
	spOrtho2D.addShaderToProgram(&shShaders[3]);
	spOrtho2D.linkProgram();

	spFont2D.createProgram();
	spFont2D.addShaderToProgram(&shShaders[2]);
	spFont2D.addShaderToProgram(&shShaders[4]);
	spFont2D.linkProgram();

	spColor.createProgram();
	spColor.addShaderToProgram(&shShaders[8]);
	spColor.addShaderToProgram(&shShaders[9]);
	spColor.linkProgram();

	// Load textures

	string sTextureNames[] = {"grass.jpg", "crate.jpg", "metalplate.jpg", "stone_wall.jpg"};

	FOR(i, NUMTEXTURES)
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
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 10.0f, 120.0f), glm::vec3(0.0f, 10.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.setMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.loadSkybox("data\\skyboxes\\jajlands1\\", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
}

/*-----------------------------------------------

Name:    renderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;

#define FOG_EQUATION_LINEAR	0
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

glm::vec3 vLightPos = glm::vec3(0.0f, 10.0f, 20.0f);

GLvoid renderColoredCube()
{
	glm::vec3 vColors[] = 
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f)
	};
	FOR(i, 6)
	{
		spColor.setUniform("vColor", glm::vec4(vColors[i].x, vColors[i].y, vColors[i].z, 1.0f));
		glDrawArrays(GL_TRIANGLES, i*6, 6);
	}
}

GLvoid renderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	spPointLights.useProgram();

	spPointLights.setUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
	spPointLights.setUniform("sunLight.fAmbientIntensity", 0.5f);
	spPointLights.setUniform("sunLight.fStrength", 0.5f);
	spPointLights.setUniform("sunLight.vDirection", glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0));

	spPointLights.setUniform("matrices.projectionMatrix", oglControl->getProjectionMatrix());
	spPointLights.setUniform("gSampler", 0);

	glm::mat4 mView = cCamera.look();
	spPointLights.setUniform("matrices.viewMatrix", &mView);
	spPointLights.setUniform("matrices.modelMatrix", glm::mat4(1.0f));

	glm::mat4 mModelMatrix = glm::translate(glm::mat4(1.0f), cCamera.vEye);
	
	spPointLights.setUniform("matrices.modelMatrix", &mModelMatrix);
	spPointLights.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mView*mModelMatrix)));
	sbMainSkybox.renderSkybox();

	glBindVertexArray(uiVAOs[0]);
	spPointLights.setUniform("sunLight.fAmbientIntensity", 0.05f);
	spPointLights.setUniform("sunLight.fStrength", 0.2f);

	spPointLights.setUniform("ptLight.vColor", glm::vec3(0.0f, 0.0f, 1.0f));
	spPointLights.setUniform("ptLight.vPosition", vLightPos);
	spPointLights.setUniform("ptLight.fAmbient", 0.15f);
	static GLfloat fConst = 0.3f, fLineaer = 0.007f, fExp = 0.00008f;
	if(Keys::key('P'))fConst += appMain.sof(0.2f);
	if(Keys::key('O'))fConst -= appMain.sof(0.2f);
	if(Keys::key('L'))fLineaer += appMain.sof(0.01f);
	if(Keys::key('K'))fLineaer -= appMain.sof(0.01f);
	if(Keys::key('M'))fExp += appMain.sof(0.0001f);
	if(Keys::key('N'))fExp -= appMain.sof(0.0001f);

	if(Keys::key(VK_LEFT))vLightPos.x -= appMain.sof(30.0f);
	if(Keys::key(VK_RIGHT))vLightPos.x += appMain.sof(30.0f);
	if(Keys::key(VK_NEXT))vLightPos.y -= appMain.sof(30.0f);
	if(Keys::key(VK_PRIOR))vLightPos.y += appMain.sof(30.0f);
	if(Keys::key(VK_UP))vLightPos.z -= appMain.sof(30.0f);
	if(Keys::key(VK_DOWN))vLightPos.z += appMain.sof(30.0f);

	spPointLights.setUniform("ptLight.fConstantAtt", fConst);
	spPointLights.setUniform("ptLight.fLinearAtt", fLineaer);
	spPointLights.setUniform("ptLight.fExpAtt", fExp);
	
	// Render ground

	spPointLights.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spPointLights.setUniform("matrices.modelMatrix", glm::mat4(1.0f));
	spPointLights.setUniform("matrices.normalMatrix", glm::mat4(1.0f));
	tTextures[0].bindTexture();
	glDrawArrays(GL_TRIANGLES, 36, 6);

	tTextures[3].bindTexture();
	glDrawArrays(GL_TRIANGLES, 42, 24);

	// Create a box pile inside "building"

	tTextures[1].bindTexture();

	SFOR(nb, 1, 9)
	{
		GLint iCnt = nb > 5 ? 10-nb : nb;
		FOR(i, iCnt)
		{
			glm::vec3 vPos = glm::vec3(-25.0f+nb*8.02f, 4.0f+i*8.02f, 0.0f);
			mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
			mModelMatrix = glm::scale(mModelMatrix, glm::vec3(8.0f, 8.0f, 8.0f));
			// We need to transform normals properly, it's done by transpose of inverse matrix of rotations and scales
			spPointLights.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
			spPointLights.setUniform("matrices.modelMatrix", mModelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	// Render 3 rotated tori to create interesting object

	tTextures[2].bindTexture();
	
	// Now it's gonna GLfloat in the air
	glm::vec3 vPos = glm::vec3(0.0f, 50.0, 0.0f);
	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	spPointLights.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spPointLights.setUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 66, iTorusFaces*3);

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	spPointLights.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spPointLights.setUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 66, iTorusFaces*3);

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle+90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	spPointLights.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spPointLights.setUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 66, iTorusFaces*3);


	spColor.useProgram();
	spColor.setUniform("matrices.projectionMatrix", oglControl->getProjectionMatrix());
	spColor.setUniform("matrices.viewMatrix", mView);

	mModelMatrix = glm::translate(glm::mat4(1.0), vLightPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));

	spColor.setUniform("matrices.modelMatrix", mModelMatrix);
	renderColoredCube();

	fGlobalAngle += appMain.sof(100.0f);
	cCamera.update();

	// PrGLint something over scene

	spFont2D.useProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.setUniform("matrices.projectionMatrix", oglControl->getOrthoMatrix());

	GLint w = oglControl->getViewportWidth(), h = oglControl->getViewportHeight();
	
	spFont2D.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.printFormatted(20, h-30, 20, "ATTENUATIONS:\nConst: %.4f\nLinear: %.4f\nExp: %.6f", fConst, fLineaer, fExp);

	glEnable(GL_DEPTH_TEST);
	if(Keys::onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->swapBuffers();
}

/*-----------------------------------------------

Name:    releaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid releaseScene(GLvoid* lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].releaseTexture();
	sbMainSkybox.releaseSkybox();

	spPointLights.deleteProgram();
	spOrtho2D.deleteProgram();
	spFont2D.deleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].deleteShader();
	ftFont.releaseFont();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.releaseVBO();
}