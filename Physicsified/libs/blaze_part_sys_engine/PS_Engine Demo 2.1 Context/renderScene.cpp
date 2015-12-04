#include "common_header.h"

#include "win_OpenGLApp.h"
#include "texture.h"
#include "flyingCamera.h"
#include "freeTypeFont.h"
#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG
#pragma comment(lib, "../lib/blaze_part_sys_engine_d.lib")
#else
#pragma comment(lib, "../lib/blaze_part_sys_engine.lib")
#endif

#include "../include/blaze_part_sys_engine.h"
#include "../include/blaze_effects.h"

#include "demoScenes.h"

BlazeParticleSystemEngine* psEngine;
CTexture tParticleTexture, tFireTexture;
CTexture tWorldTextures[NUMTEXTURES];
CFlyingCamera fcMain;

CSkybox sbSkyboxes[NUMSKYBOXES];

CFreeTypeFont ftFont;

/*-----------------------------------------------

Name:		initScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
			be used.

/*---------------------------------------------*/

void initScene(LPVOID lpParam)
{
	glEnable(GL_DEPTH_TEST); // Enables depth-testing
	glShadeModel(GL_SMOOTH); // Enables smooth shading
	glDepthFunc(GL_LESS);	// The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	psEngine = createSimpleParticleSystem();
	//psEngine = createPointSpritesParticleSystem();
	psEngine->initalizeParticleSystem();

	tFireTexture.loadTexture2D("data\\textures\\fire.bmp", true, true);
	tFireTexture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST_MIPMAP);

	tParticleTexture.loadTexture2D("data\\textures\\particle.bmp", true, true);
	tParticleTexture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST_MIPMAP);

	ftFont.loadSystemFont("arial.ttf", 32);

	fcMain.setMovingKeys('W', 'S', 'A', 'D');

	string sTextureNames[] = {"grass.jpg", "stone_wall.jpg", "octlig01bc.jpg", "water2.jpg", "marble.jpg", "marble2.jpg"};
	FOR(i, NUMTEXTURES)
	{
		tWorldTextures[i].loadTexture2D(string("data\\textures\\")+sTextureNames[i], true);
		tWorldTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	sbSkyboxes[0].loadSkybox("data\\skyboxes\\jajlands1\\", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
	sbSkyboxes[1].loadSkybox("data\\skyboxes\\jajspace2\\", "jajspace2_ft.jpg", "jajspace2_bk.jpg", "jajspace2_lf.jpg", "jajspace2_rt.jpg", "jajspace2_up.jpg", "jajspace2_dn.jpg");
	sbSkyboxes[2].loadSkybox("data\\skyboxes\\jajalien1\\", "jajalien1_ft.jpg", "jajalien1_bk.jpg", "jajalien1_lf.jpg", "jajalien1_rt.jpg", "jajalien1_up.jpg", "jajalien1_dn.jpg");
}

/*-----------------------------------------------

Name:	renderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

float fGlobalAngle;
float fDryAmount = 0.75f;

bool bMenu = true;
int iSelectedDemo = 0;

void renderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	int w = oglControl->getViewportWidth();
	int h = oglControl->getViewportHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(bMenu)
	{
		glColor3ub(255, 255, 255);
		ftFont.print("Press:\n\n1.) For Fire Demo Scene\n2.) For In Space Demo Scene\n3.) For Fountain Demo Scene", 20, h-30, 24, true);

		if(Keys::onekey('1') || Keys::onekey(VK_NUMPAD1))
		{
			iSelectedDemo = 0;
			prepareFireDemoScene();
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
		fcMain.look();
		psEngine->setLookAt(fcMain.vEye.x, fcMain.vEye.y, fcMain.vEye.z, fcMain.vView.x, fcMain.vView.y, fcMain.vView.z, fcMain.vUp.x, fcMain.vUp.y, fcMain.vUp.z);
		if(iSelectedDemo == 0)renderFireDemoScene();
		else if(iSelectedDemo == 1)renderSpaceDemoScene();
		else if(iSelectedDemo == 2)renderFountainDemoScene();
		if(Keys::onekey(VK_ESCAPE))
		{
			bMenu = true;
			psEngine->clearAllGenerators();
			psEngine->clearAllParticles();
		}
		if(Keys::onekey('H'))
		{
			BlazeEffects::deleteEffect(4);
		}
		fcMain.update();
		glColor3ub(255, 255, 255);

		char buf[255];
		sprintf(buf, "Particles on scene: %d", psEngine->getNumParticles());
		ftFont.print(buf, 20, h-30, 24, true);
		sprintf(buf, "Generators: %d", psEngine->getNumGenerators());
		ftFont.print(buf, 20, h-60, 24, true);
		sprintf(buf, "FPS: %d", oglControl->getFPS());
		ftFont.print(buf, 20, h-90, 24, true);
	}
	oglControl->swapBuffers();
}

/*-----------------------------------------------

Name:	releaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

void releaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tWorldTextures[i].releaseTexture();
	tParticleTexture.releaseTexture();
	tFireTexture.releaseTexture();
	FOR(i, NUMSKYBOXES)sbSkyboxes[i].releaseSkybox();

	psEngine->releaseParticleSystem();
	delete psEngine;
}