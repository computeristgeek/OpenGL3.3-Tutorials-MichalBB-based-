#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"
#include "dirLight.h"
#include "material.h"

#include "static_geometry.h"

#include "heightmap.h"

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;
CMaterial matShiny;

CMultiLayeredHeightmap hmWorld;

/*-----------------------------------------------

Name:    InitScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
         be used.

/*---------------------------------------------*/

void InitScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if(!PrepareShaderPrograms())
	{
		PostQuitMessage(0);
		return;
	}
	
	LoadAllTextures();

	PrepareStaticSceneObjects();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we print with size above 32, the quality will be low
	// Second parameter - 1024 says, how many characters do we want to support (it must not be necessarily power of 2, you can go for 1050 or whatever you want, it's just
	// a multiple of 256, which will result in 4 textures filled with characters)
	// We can go further, for example symbols like euro (€) with code 8364 need more, go for 10000 to test it :)
	
	// I copied font from my Windows/Fonts folder, I just wanted to make sure that the slovak and german characters would be displayed on every computer
	// If you are from USA or Canada, you can try to load system font and see, whether your system would be able to display these characters and then let me know :)
	
	//ftFont.LoadSystemFont("arial.ttf", 32, 1024);
	ftFont.LoadFont("arial.ttf", 32, 1024);

	ftFont.SetShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 30.0f, 99.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\jajlands1\\", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f, 0);
	
	matShiny = CMaterial(1.0f, 32.0f);

	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.SetRenderSize(300.0f, 35.0f, 300.0f);
	hmWorld.LoadHeightMapFromImage("data\\worlds\\world_like_in_21th_tutorial.bmp");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

float fAlphaTest = 0.25f;
float fAlphaMultiplier = 1.5f;

void RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	oglControl->ResizeOpenGLViewportFull();

	glm::mat4 mModel;

	// Render skybox

	spSkybox.UseProgram();

	spSkybox.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spSkybox.SetUniform("matrices.viewMatrix", cCamera.Look());
	spSkybox.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));

	spSkybox.SetUniform("vColor", glm::vec4(1, 1, 1, 1));
	spSkybox.SetUniform("gSampler", 0);	
	spSkybox.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::translate(glm::mat4(1.0), cCamera.vEye));

	sbMainSkybox.RenderSkybox();

	spMain.UseProgram();

	spMain.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spMain.SetUniform("matrices.viewMatrix", cCamera.Look());

	spMain.SetUniform("gSampler", 0);
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	// This values set the darkness of whole scene (direction of light), that's why such name of variable :D
	static float fAngleOfDarkness = 45.0f;
	// You can play with direction of light with '+' and '-' key
	if(Keys::Key(VK_ADD))fAngleOfDarkness += appMain.sof(90);
	if(Keys::Key(VK_SUBTRACT))fAngleOfDarkness -= appMain.sof(90);
	// Set the directional vector of light
	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f/180.0f), -cos(fAngleOfDarkness*3.1415f/180.0f), 0.0f);
	dlSun.SetUniformData(&spMain, "sunLight");

	spMain.SetUniform("vEyePosition", cCamera.vEye);
	// I'm always using this shiny material, no matter what I render, it would be nice to change it sometimes :P
	matShiny.SetUniformData(&spMain, "matActive"); 

	spMain.UseProgram();
	glBindVertexArray(uiVAOSceneObjects);

	// Render that grid of where spheres are

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::translate(glm::mat4(1.0), glm::vec3(0.0f, fCubeHalfSize, 0.0f)));
	tTextures[6].BindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Now we're going to render terrain

	CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();
	CShaderProgram* spGrass = CMultiLayeredHeightmap::GetGrassShaderProgram();

	spTerrain->UseProgram();

	spTerrain->SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spTerrain->SetUniform("matrices.viewMatrix", cCamera.Look());
	spTerrain->SetUniform("vEyePosition", cCamera.vEye);

	matShiny.SetUniformData(spTerrain, "matActive");

	// We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
	// the places in heightmap where path should be and how intense should it be
	FOR(i, 5)
	{
		char sSamplerName[256];
		sprintf(sSamplerName, "gSampler[%d]", i);
		tTextures[i].BindTexture(i);
		spTerrain->SetUniform(sSamplerName, i);
	}

	// ... set some uniforms
	spTerrain->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	spTerrain->SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	dlSun.SetUniformData(spTerrain, "sunLight");

	spGrass->UseProgram();

	spGrass->SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spGrass->SetUniform("matrices.viewMatrix", cCamera.Look());
	spGrass->SetUniform("vEyePosition", cCamera.vEye);

	// ... set some uniforms
	spGrass->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	spGrass->SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	spGrass->SetUniform("fAlphaTest", fAlphaTest);
	spGrass->SetUniform("fAlphaMultiplier", fAlphaMultiplier);

	// ... and finally render heightmap
	hmWorld.RenderHeightmap();

	cCamera.Update();

	// Print something over scene
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	int w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.PrintFormatted(20, h-20, 24, "FPS: %d", oglControl->GetFPS());
	ftFont.PrintFormatted(20, h-80, 24, "Alpha test value: %.2f (Press 'R' and 'E' to change)", fAlphaTest);
	ftFont.PrintFormatted(20, h-105, 24, "Alpha multiplier: %.2f (Press 'X' and 'Z' to change)", fAlphaMultiplier);

	if(Keys::Key('R'))
		fAlphaTest += appMain.sof(0.1f);
	if(Keys::Key('E'))
		fAlphaTest -= appMain.sof(0.1f);

	if(Keys::Key('X'))
		fAlphaMultiplier += appMain.sof(0.1f);
	if(Keys::Key('Z'))
		fAlphaMultiplier -= appMain.sof(0.1f);

	glEnable(GL_DEPTH_TEST);	
	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to OpenGL control.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

void ReleaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	sbMainSkybox.DeleteSkybox();

	DeleteAllShaderPrograms();

	ftFont.DeleteFont();

	glDeleteVertexArrays(1, &uiVAOSceneObjects);
	vboSceneObjects.DeleteVBO();
}