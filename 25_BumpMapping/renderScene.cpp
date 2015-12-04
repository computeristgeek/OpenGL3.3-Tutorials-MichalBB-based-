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

#include "assimp_model.h"
#include "md2model.h"

#include "heightmap.h"

#include "static_geometry.h"

#include "particle_system_tf.h"

CVertexBufferObject vboSceneObjects;
UINT uiVAOSceneObjects;

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;

CMaterial matShiny;
CAssimpModel amModels[2];

CMultiLayeredHeightmap hmWorld;

GLint iTorusFaces;

GLboolean bDisplayNormals = false; // Do not display normals by default

CParticleSystemTransformFeedback psMainParticleSystem;
CMD2Model md2Models[1];
animState_t animationStateMain;

GLint bEnableBumpMap = 1;

/*-----------------------------------------------

Name:    InitScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
         be used.

/*---------------------------------------------*/

GLvoid InitScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if(!PrepareShaderPrograms())
	{
		PostQuitMessage(0);
		return;
	}
	
	LoadAllTextures();

	vboSceneObjects.CreateVBO();
	glGenVertexArrays(1, &uiVAOSceneObjects); // Create one VAO
	glBindVertexArray(uiVAOSceneObjects);

	vboSceneObjects.BindVBO();

	iTorusFaces = GenerateTorus(vboSceneObjects, 7.0f, 2.0f, 20, 20);
	vboSceneObjects.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));


	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.LoadSystemFont("arial.ttf", 32);
	ftFont.SetShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 30.0f, 99.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\clouds\\", "clouds_front.jpg", "clouds_back.jpg", "clouds_right.jpg", "clouds_left.jpg", "clouds_top.jpg", "clouds_top.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f, 0);

	amModels[0].LoadModelFromFile("data\\models\\ColRuins\\Ruins.obj", "DiffuseMap.tga", "NormalMap.tga");
	amModels[1].LoadModelFromFile("data\\models\\StoneWall\\stone_wall.obj", "stone_wall.bmp", "stone_wall_normal_map.bmp");
	
	CAssimpModel::FinalizeVBO();
	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("data\\worlds\\world.bmp");
	hmWorld.SetRenderSize(300.0f, 25.0f, 300.0f);

	matShiny = CMaterial(1.0f, 32.0f);

	psMainParticleSystem.InitalizeParticleSystem();

	psMainParticleSystem.SetGeneratorProperties(
		glm::vec3(-120.0f, 28.1f, -33), // Where the particles are generated
		glm::vec3(-10, 0, -10), // Minimal velocity
		glm::vec3(10, 20, 10), // Maximal velocity
		glm::vec3(0, -10, 0), // Gravity force applied to particles
		glm::vec3(0.0f, 0.5f, 1.0f), // Color (light blue)
		1.5f, // Minimum lifetime in seconds
		3.0f, // Maximum lifetime in seconds
		0.25f, // Rendered size
		0.02f, // Spawn every 0.02 seconds
		30); // And spawn 30 particles

	md2Models[0].LoadModel("data/models/drfreak/drfreak.md2");
	animationStateMain = md2Models[0].StartAnimation(STAND);
}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

glm::vec3 vModelPosition = glm::vec3(0, 20, 0);
GLfloat fModelRotation;
animType_t atCurrentAnimation = STAND;

GLvoid RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
	oglControl->ResizeOpenGLViewportFull();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));
	// This values set the darkness of whole scene, that's why such name of variable :D
	static GLfloat fAngleOfDarkness = 189.18f;
	// You can play with direction of light with '+' and '-' key
	if(Keys::Key(VK_ADD))fAngleOfDarkness += appMain.sof(90);
	if(Keys::Key(VK_SUBTRACT))fAngleOfDarkness -= appMain.sof(90);

	// Set the directional vector of light
	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f/180.0f), -cos(fAngleOfDarkness*3.1415f/180.0f), 0.0f);
	dlSun.SetUniformData(&spMain, "sunLight");

	spMain.SetUniform("vEyePosition", cCamera.vEye);
	matShiny.SetUniformData(&spMain, "matActive");

	glm::mat4 mModel = glm::translate(glm::mat4(1.0), glm::vec3(vModelPosition));
	mModel = glm::rotate(mModel, fModelRotation, glm::vec3(0, 1, 0));
	mModel = glm::rotate(mModel, -90.0f, glm::vec3(1, 0, 0));
	mModel = glm::scale(mModel, glm::vec3(0.35f, 0.35f, 0.35f));

	spMD2Animation.UseProgram();
	spMD2Animation.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spMD2Animation.SetUniform("matrices.viewMatrix", cCamera.Look());

	tTextures[2].BindTexture();
	spMD2Animation.SetUniform("gSampler", 0);
	spMD2Animation.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	dlSun.SetUniformData(&spMD2Animation, "sunLight");
	matShiny.SetUniformData(&spMD2Animation, "matActive");

	spMD2Animation.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

	md2Models[0].RenderModel(&animationStateMain);
	md2Models[0].UpdateAnimation(&animationStateMain, appMain.sof(1.0f));

	CAssimpModel::BindModelsVAO();

	spMain.UseProgram();
	
	// This line enables bump mapping calculations in our main shader program
	spMain.SetUniform("bEnableBumpMap", bEnableBumpMap);
	// We just tell the shader, that texture unit 1 has normal map
	spMain.SetUniform("gNormalMap", 1);

	GLfloat fRuinsScaleFactor = 0.85f;

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(-150.0f, 0.1f, -150));
	//mModel = glm::rotate(mModel, fAngleOfDarkness, glm::vec3(0, 1, 0));
	mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

	amModels[0].RenderModel();

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(150.0f, 0.1f, -150));
	mModel = glm::rotate(mModel, -90.0f, glm::vec3(0, 1, 0));
	mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

	amModels[0].RenderModel();

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(150.0f, 0.1f, 150));
	mModel = glm::rotate(mModel, -180.0f, glm::vec3(0, 1, 0));
	mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

	amModels[0].RenderModel();


	mModel = glm::translate(glm::mat4(1.0), glm::vec3(-150.0f, 0.1f, 150));
	mModel = glm::rotate(mModel, -270.0f, glm::vec3(0, 1, 0));
	mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

	amModels[0].RenderModel();

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(110.0f, 40.1f, -60));
	mModel = glm::scale(mModel, glm::vec3(3.0f, 2.0f, 2.5f));
	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

	amModels[1].RenderModel();

	// Now we're going to render terrain

	CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();

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

	// ... and finally render heightmap
	hmWorld.RenderHeightmap();

	if(bDisplayNormals)
	{
		spNormalDisplayer.UseProgram();
		spNormalDisplayer.SetUniform("fNormalLength", 1.0f);
		spNormalDisplayer.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
		spNormalDisplayer.SetUniform("matrices.viewMatrix", cCamera.Look());

		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", hmWorld.GetScaleMatrix());
		hmWorld.RenderHeightmapForNormals();

		CAssimpModel::BindModelsVAO();

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(-150.0f, 0.1f, -150));
		//mModel = glm::rotate(mModel, fAngleOfDarkness, glm::vec3(0, 1, 0));
		mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

		spNormalDisplayer.SetUniform("fNormalLength", 10.0f);

		amModels[0].RenderModel(GL_POINTS);

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(150.0f, 0.1f, -150));
		mModel = glm::rotate(mModel, -90.0f, glm::vec3(0, 1, 0));
		mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

		amModels[0].RenderModel(GL_POINTS);

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(150.0f, 0.1f, 150));
		mModel = glm::rotate(mModel, -180.0f, glm::vec3(0, 1, 0));
		mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

		amModels[0].RenderModel(GL_POINTS);

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(-150.0f, 0.1f, 150));
		mModel = glm::rotate(mModel, -270.0f, glm::vec3(0, 1, 0));
		mModel = glm::scale(mModel, glm::vec3(fRuinsScaleFactor, fRuinsScaleFactor, fRuinsScaleFactor));
		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

		amModels[0].RenderModel(GL_POINTS);

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(-20.0f, 25.1f, -33));
		mModel = glm::scale(mModel, glm::vec3(0.5f, 0.5f, 0.5f));
		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(-20.0f, 25.1f, -33));
		mModel = glm::scale(mModel, glm::vec3(0.5f, 0.5f, 0.5f));
		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);

		amModels[1].RenderModel(GL_POINTS);
	}

	// Render particles

	psMainParticleSystem.SetGeneratorPosition(vModelPosition);

	tTextures[6].BindTexture(); 

	psMainParticleSystem.SetMatrices(oglControl->GetProjectionMatrix(), cCamera.vEye, cCamera.vView, cCamera.vUp);
	psMainParticleSystem.UpdateParticles(appMain.sof(1.0f));
	psMainParticleSystem.RenderParticles();

	cCamera.Update();

	// PrGLint something over scene
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	GLint w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);
	ftFont.PrintFormatted(20, h-30, 20, "FPS: %d", oglControl->GetFPS());
	ftFont.PrintFormatted(20, h-55, 20, "Particles: %d", psMainParticleSystem.GetNumParticles());
	ftFont.PrintFormatted(20, h-80, 20, "Displaying Normals: %s (Press 'N' to toggle)", bDisplayNormals ? "Yes" : "Nope");

	ftFont.PrintFormatted(20, h-130, 20, "Bump Mapping: %s (Press 'B' to toggle)", bEnableBumpMap ? "Enabled" : "Disabled");

	// Handle input

	GLboolean bRunning = false;
	static GLboolean bLastFrameRunning = false;

	if(Keys::Key(VK_UP))
	{
		GLfloat fSine = float(sin((fModelRotation+90)*3.1415f/180.0f));
		GLfloat fCosine = float(cos((fModelRotation+90)*3.1415f/180.0f));

		glm::vec3 vMoveVector(fSine, 0, fCosine);

		vModelPosition += vMoveVector*appMain.sof(30.0f);

		if(animationStateMain.type != RUN)animationStateMain = md2Models[0].StartAnimation(RUN);
		bRunning = true;
	}
	if(Keys::Key(VK_DOWN))
	{
		GLfloat fSine = float(sin((fModelRotation+90)*3.1415f/180.0f));
		GLfloat fCosine = float(cos((fModelRotation+90)*3.1415f/180.0f));

		glm::vec3 vMoveVector(fSine, 0, fCosine);

		vModelPosition -= vMoveVector*appMain.sof(30.0f);

		if(animationStateMain.type != RUN)animationStateMain = md2Models[0].StartAnimation(RUN);
		bRunning = true;
	}
	if(Keys::Key(VK_LEFT))
		fModelRotation += appMain.sof(135.0f);
	if(Keys::Key(VK_RIGHT))
		fModelRotation -= appMain.sof(135.0f);

	vModelPosition.y = hmWorld.GetHeightFromRealVector(vModelPosition)+8.0f;

	if(!bRunning && bLastFrameRunning)animationStateMain = md2Models[0].StartAnimation(atCurrentAnimation);
	bLastFrameRunning = bRunning;

	psMainParticleSystem.SetNumberToGenerate(bRunning ? 30 : 0);

	if(Keys::Onekey('Z'))
	{
		atCurrentAnimation = animType_t( (int(atCurrentAnimation) + int(MAX_ANIMATIONS - 1) - 1) % int(MAX_ANIMATIONS - 1));
		animationStateMain = md2Models[0].StartAnimation(atCurrentAnimation);
	}

	if(Keys::Onekey('X'))
	{
		atCurrentAnimation = animType_t( (int(atCurrentAnimation) + 1) % int(MAX_ANIMATIONS - 1));
		animationStateMain = md2Models[0].StartAnimation(atCurrentAnimation);
	}

	if(Keys::Onekey('B'))bEnableBumpMap = 1 - bEnableBumpMap;

	if(Keys::Onekey('N'))bDisplayNormals = !bDisplayNormals;

	if(Keys::Onekey('R'))tTextures[4].ReloadTexture();

	glEnable(GL_DEPTH_TEST);	
	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	sbMainSkybox.DeleteSkybox();

	spMain.DeleteProgram();
	spOrtho2D.DeleteProgram();
	spFont2D.DeleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].DeleteShader();
	ftFont.DeleteFont();

	glDeleteVertexArrays(1, &uiVAOSceneObjects);
	vboSceneObjects.DeleteVBO();

	hmWorld.ReleaseHeightmap();
	CMultiLayeredHeightmap::ReleaseTerrainShaderProgram();

	psMainParticleSystem.ReleaseParticleSystem();
}