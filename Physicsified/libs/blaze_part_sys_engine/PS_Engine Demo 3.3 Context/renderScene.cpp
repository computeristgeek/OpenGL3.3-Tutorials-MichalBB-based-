#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"

#include "demoScenes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboFireScene, vboSpaceScene, vboFountainScene;
UINT uiVAOs[NUMVAOS];

CShader shShaders[NUMSHADERS];
CShaderProgram spDirectionalLight, spOrtho2D, spFont2D, spColor;

CTexture tFireTexture, tParticleTexture;
CTexture tWorldTextures[NUMTEXTURES];
CFlyingCamera fcMain;

CSkybox sbSkyboxes[NUMSKYBOXES];

CFreeTypeFont ftFont;

#ifdef _DEBUG
#pragma comment(lib, "../lib/blaze_part_sys_engine_d.lib")
#else
#pragma comment(lib, "../lib/blaze_part_sys_engine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "../lib/blaze_effects_d.lib")
#else
#pragma comment(lib, "../lib/blaze_part_sys_engine.lib")
#endif

#include "demoScenes.h"

BlazeParticleSystemEngine* psEngine;

/*-----------------------------------------------

Name:		initScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
			be used.

/*---------------------------------------------*/

#include "static_geometry.h"

int iTorusFaces;

void initScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	vboFireScene.createVBO();
	vboSpaceScene.createVBO();
	vboFountainScene.createVBO();
	glGenVertexArrays(NUMVAOS, uiVAOs);

	vboFireScene.bindVBO();
	
	// Add ground to VBO

	FOR(i, 6)
	{
		vboFireScene.addData(&vGround[i], sizeof(glm::vec3));
		glm::vec2 vTexCoord = vCubeTexCoords[i] *10.0f;
		vboFireScene.addData(&vTexCoord, sizeof(glm::vec2));
	}

	generateWalls(vboFireScene, glm::vec3(-200.0f, 0.0f, 200.0f), glm::vec3(0.0f, 0.0f, -1.0f), 400.0f, 4.0f, 30.0f);
	generateWalls(vboFireScene, glm::vec3(-200.0f, 0.0f, -200.0f), glm::vec3(1.0f, 0.0f, 0.0f), 400.0f, 4.0f, 30.0f);
	generateWalls(vboFireScene, glm::vec3(200.0f, 0.0f, -200.0f), glm::vec3(0.0f, 0.0f, 1.0f), 400.0f, 4.0f, 30.0f);
	generateWalls(vboFireScene, glm::vec3(200.0f, 0.0f, 200.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 400.0f, 4.0f, 30.0f);
	
	vboFireScene.uploadDataToGPU(GL_STATIC_DRAW);

	vboSpaceScene.bindVBO();

	generateWalls(vboSpaceScene, glm::vec3(40.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 400.5f, 30.0f, 22.0f);
	generateWalls(vboSpaceScene, glm::vec3(-40.0f, 0.0f, -400.0f), glm::vec3(0.0f, 0.0f, 1.0f), 400.5f, 30.0f, 22.0f);

	FOR(i, 36)
	{
		vboSpaceScene.addData(&vCubeVertices[i], sizeof(glm::vec3));
		glm::vec2 vTexCoordDummy;
		vboSpaceScene.addData(&vTexCoordDummy, sizeof(glm::vec2));
	}

	vboSpaceScene.uploadDataToGPU(GL_STATIC_DRAW);
	vboFountainScene.bindVBO();

	FOR(y, 2)
	{
		FOR(i, 6)
		{
			glm::vec3 vGroundVertex = vGround[i]*0.5f;
			vGroundVertex.y -= y*10.0f;
			vboFountainScene.addData(&vGroundVertex, sizeof(glm::vec3));
			glm::vec2 vTexCoord = vCubeTexCoords[i]*20.0f;
			vboFountainScene.addData(&vTexCoord, sizeof(glm::vec2));
		}
	}

	generateWalls(vboFountainScene, glm::vec3(-100.0f, -10.0f, 100.0f), glm::vec3(0.0f, 0.0f, -1.0f), 200.0f, 4.0f, 30.0f);
	generateWalls(vboFountainScene, glm::vec3(-100.0f, -10.0f, -100.0f), glm::vec3(1.0f, 0.0f, 0.0f), 200.0f, 4.0f, 30.0f);
	generateWalls(vboFountainScene, glm::vec3(100.0f, -10.0f, -100.0f), glm::vec3(0.0f, 0.0f, 1.0f), 200.0f, 4.0f, 30.0f);
	generateWalls(vboFountainScene, glm::vec3(100.0f, -10.0f, 100.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 200.0f, 4.0f, 30.0f);

	vboFountainScene.uploadDataToGPU(GL_STATIC_DRAW);

	FOR(i, NUMVAOS)
	{
		glBindVertexArray(uiVAOs[i]);

		if(i == 0)vboFireScene.bindVBO();
		else if(i == 1)vboSpaceScene.bindVBO();
		else if(i == 2)vboFountainScene.bindVBO();

		// Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), 0);
		// Texture coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	}

	// Load shaders and create shader programs

	shShaders[0].loadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shShaders[1].loadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);
	shShaders[2].loadShader("data\\shaders\\ortho2D.vert", GL_VERTEX_SHADER);
	shShaders[3].loadShader("data\\shaders\\ortho2D.frag", GL_FRAGMENT_SHADER);
	shShaders[4].loadShader("data\\shaders\\font2D.frag", GL_FRAGMENT_SHADER);
	shShaders[5].loadShader("data\\shaders\\color.vert", GL_VERTEX_SHADER);
	shShaders[6].loadShader("data\\shaders\\color.frag", GL_FRAGMENT_SHADER);

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

	spColor.createProgram();
	spColor.addShaderToProgram(&shShaders[5]);
	spColor.addShaderToProgram(&shShaders[6]);
	spColor.linkProgram();

	// Load textures

	string sTextureNames[] = {"grass.jpg", "stone_wall.jpg", "fire.bmp", "particle.bmp"};

	FOR(i, NUMTEXTURES) // I know that FOR cycle is useless now, but it was easier to rewrite :)
	{
		tWorldTextures[i].loadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tWorldTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Here we load font with pixel size 32 - this means that if we print with size above 32, the quality will be low
	ftFont.loadSystemFont("arial.ttf", 32);
	ftFont.setShaderProgram(&spFont2D);
	
	fcMain = CFlyingCamera(glm::vec3(0.0f, 30.0f, 30.0f), glm::vec3(0.0f, 30.0f, 29.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	fcMain.setMovingKeys('W', 'S', 'A', 'D');

	sbSkyboxes[0].loadSkybox("data\\skyboxes\\jajlands1\\", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
	sbSkyboxes[1].loadSkybox("data\\skyboxes\\jajspace2\\", "jajspace2_ft.jpg", "jajspace2_bk.jpg", "jajspace2_lf.jpg", "jajspace2_rt.jpg", "jajspace2_up.jpg", "jajspace2_dn.jpg");
	sbSkyboxes[2].loadSkybox("data\\skyboxes\\jajalien1\\", "jajalien1_ft.jpg", "jajalien1_bk.jpg", "jajalien1_lf.jpg", "jajalien1_rt.jpg", "jajalien1_up.jpg", "jajalien1_dn.jpg");

	psEngine = createTransformFeedbackParticleSystem();
	psEngine->initalizeParticleSystem();

	tFireTexture.loadTexture2D("data\\textures\\fire.bmp", true);
	tFireTexture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST);
	tParticleTexture.loadTexture2D("data\\textures\\particle.bmp", true);
	tParticleTexture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST);
}

/*-----------------------------------------------

Name:	renderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

bool bMenu = true;
int iSelectedDemo = 0;

float fGlobalAngle;

void renderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	int w = oglControl->getViewportWidth();
	int h = oglControl->getViewportHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(bMenu)
	{
		spFont2D.useProgram();
		glDisable(GL_DEPTH_TEST);
		spFont2D.setUniform("projectionMatrix", oglControl->getOrthoMatrix());
		spFont2D.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		ftFont.print("Press:\n\n1.) For Fire Demo Scene\n2.) For In Space Demo Scene\n3.) For Fountain Demo Scene", 20, h-30, 24);

		if(Keys::onekey('1') || Keys::onekey(VK_NUMPAD1))
		{
			iSelectedDemo = 0;
			prepareFireDemoScene();
			GLenum error = glGetError();
			fcMain = CFlyingCamera(glm::vec3(0.0f, 30.0f, 30.0f), glm::vec3(0.0f, 30.0f, 29.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
			fcMain.setMovingKeys('W', 'S', 'A', 'D');
			bMenu = false;
		}
		if(Keys::onekey('2') || Keys::onekey(VK_NUMPAD2))
		{
			iSelectedDemo = 1;
			prepareSpaceDemoScene();
			fcMain = CFlyingCamera(glm::vec3(0.0f, 30.0f, 30.0f), glm::vec3(0.0f, 30.0f, 29.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
			fcMain.setMovingKeys('W', 'S', 'A', 'D');
			bMenu = false;
		}
		if(Keys::onekey('3') || Keys::onekey(VK_NUMPAD3))
		{
			iSelectedDemo = 2;
			prepareFountainDemoScene();
			fcMain = CFlyingCamera(glm::vec3(0.0f, 30.0f, 30.0f), glm::vec3(0.0f, 30.0f, 29.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
			fcMain.setMovingKeys('W', 'S', 'A', 'D');
			bMenu = false;
		}
	}
	if(!bMenu)
	{
		spDirectionalLight.useProgram();
		spDirectionalLight.setUniform("fAddToXTextureCoordinate", 0.0f);

		if(iSelectedDemo == 0)renderFireDemoScene(oglControl);
		else if(iSelectedDemo == 1)renderSpaceDemoScene(oglControl);
		else if(iSelectedDemo == 2)renderFountainDemoScene(oglControl);
		
		fcMain.update();
		

		if(Keys::onekey(VK_ESCAPE))
		{
			bMenu = true;
			psEngine->clearAllGenerators();
			psEngine->clearAllParticles();
			BlazeEffects::deleteAllEffects();
		}

		spFont2D.useProgram();
		glDisable(GL_DEPTH_TEST);
		spFont2D.setUniform("projectionMatrix", oglControl->getOrthoMatrix());
		spFont2D.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		char buf[255];
		sprintf(buf, "Particles on scene: %d", psEngine->getNumParticles());
		ftFont.print(buf, 20, h-30, 24);
		sprintf(buf, "Generators: %d", psEngine->getNumGenerators());
		ftFont.print(buf, 20, h-60, 24);
		sprintf(buf, "FPS: %d", oglControl->getFPS());
		ftFont.print(buf, 20, h-90, 24);

		glEnable(GL_DEPTH_TEST);
	}

	if(Keys::onekey(VK_ESCAPE))
	{
		bMenu = true;
	}

	oglControl->swapBuffers();

	if(Keys::onekey('R'))psEngine->clearAllParticles();
	if(Keys::onekey('G'))psEngine->clearAllGenerators();
}

/*-----------------------------------------------

Name:	releaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

void releaseScene(LPVOID lpParam)
{
	psEngine->releaseParticleSystem();
	delete psEngine;

	FOR(i, NUMTEXTURES)tWorldTextures[i].releaseTexture();
	FOR(i, NUMSKYBOXES)sbSkyboxes[i].releaseSkybox();

	spDirectionalLight.deleteProgram();
	spOrtho2D.deleteProgram();
	spFont2D.deleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].deleteShader();
	ftFont.releaseFont();

	glDeleteVertexArrays(NUMVAOS, uiVAOs);
	vboFireScene.releaseVBO();
}