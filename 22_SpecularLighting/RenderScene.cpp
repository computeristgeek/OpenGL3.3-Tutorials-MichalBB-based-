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

#include "heightmap.h"

#include "static_geometry.h"


CVertexBufferObject vboSceneObjects;
GLuint uiVAOSceneObjects;

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;

CMaterial matShiny;
CAssimpModel amModels[3];

CMultiLayeredHeightmap hmWorld;

GLint iTorusFaces;

GLboolean bDisplayNormals = true; // Do not display normals by default

/*-----------------------------------------------

Name:    InitScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
         be used.

/*---------------------------------------------*/

GLvoid InitScene(GLvoid* lpParam)
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
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 30.0f, 99.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\nuke\\", "nuke_front.jpg", "nuke_back.jpg", "nuke_right.jpg", "nuke_left.jpg", "nuke_top.jpg", "nuke_top.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f, 0);

	amModels[0].LoadModelFromFile("data\\models\\house\\house.3ds");
	amModels[1].LoadModelFromFile("data\\models\\small_fighter_1\\small_fighter_1.3DS");
	
	CAssimpModel::FinalizeVBO();


	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("data\\worlds\\world_like_in_21th_tutorial.bmp");

	matShiny = CMaterial(1.0f, 32.0f);
}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
	oglControl->ResizeOpenGLViewportFull();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

	spMain.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spMain.SetUniform("matrices.viewMatrix", cCamera.Look());

	spMain.SetUniform("gSampler", 0);

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	// This values will set the darkness of whole scene, that's why such name of variable :D
	static GLfloat fAngleOfDarkness = 45.0f;
	// You can play with direction of light with '+' and '-' key
	if(Keys::Key(VK_ADD))fAngleOfDarkness += appMain.sof(90);
	if(Keys::Key(VK_SUBTRACT))fAngleOfDarkness -= appMain.sof(90);
	// Set the directional vector of light
	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f/180.0f), -cos(fAngleOfDarkness*3.1415f/180.0f), 0.0f);
	
	dlSun.iSkybox = 1;
	dlSun.SetUniformData(&spMain, "sunLight");

	spMain.SetUniform("matrices.modelMatrix", glm::translate(glm::mat4(1.0), cCamera.vEye));
	sbMainSkybox.RenderSkybox();

	dlSun.iSkybox = 0;
	dlSun.SetUniformData(&spMain, "sunLight");

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));

	spMain.SetUniform("vEyePosition", cCamera.vEye);
	matShiny.SetUniformData(&spMain, "matActive");
	
	// Render a house

	CAssimpModel::BindModelsVAO();

	glm::mat4 mModel = glm::translate(glm::mat4(1.0), glm::vec3(40.0f, 17.0f, 0));
	mModel = glm::scale(mModel, glm::vec3(8, 8, 8));

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
	amModels[0].RenderModel();
	
	// ... and a jet fighter

	mModel = glm::translate(glm::mat4(1.0), glm::vec3(-20.0f, 72.0f, 50));
	mModel = glm::rotate(mModel, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	mModel = glm::scale(mModel, glm::vec3(1, 1, 1));

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
	amModels[1].RenderModel();

	// Render 3 rotated tori to create interesting object

	tTextures[5].BindTexture();
	glBindVertexArray(uiVAOSceneObjects);
	static GLfloat fGlobalAngle = 0.0f;

	FOR(i, 2)
	{
		glm::vec3 vCenter = glm::vec3(-40+i*40, 30, -20);
		mModel = glm::translate(glm::mat4(1.0), vCenter);
		if(i == 0)mModel = glm::rotate(mModel, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
		glDrawArrays(GL_TRIANGLES, 0, iTorusFaces*3);

		mModel = glm::translate(glm::mat4(1.0), vCenter+glm::vec3(0.01f, 0.0f, 0.0f));
		if(i == 0)mModel = glm::rotate(mModel, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		mModel = glm::rotate(mModel, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
		glDrawArrays(GL_TRIANGLES, 0, iTorusFaces*3);

		mModel = glm::translate(glm::mat4(1.0), vCenter+glm::vec3(0.00f, 0.01f, 0.0f));

		if(i == 0)mModel = glm::rotate(mModel, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		mModel = glm::rotate(mModel, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
		glDrawArrays(GL_TRIANGLES, 0, iTorusFaces*3);
	}

	fGlobalAngle += appMain.sof(50.0f);


	// Now we're going to render terrain

	hmWorld.SetRenderSize(300.0f, 35.0f, 300.0f);
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
		GLchar sSamplerName[256];
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

		CAssimpModel::BindModelsVAO();

		// ... Render the house again

		glm::mat4 mModel = glm::translate(glm::mat4(1.0), glm::vec3(40.0f, 17.0f, 0));
		mModel = glm::scale(mModel, glm::vec3(8, 8, 8));

		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
		amModels[0].RenderModel(GL_POINTS);

		// ... and the jet fighter again

		mModel = glm::translate(glm::mat4(1.0), glm::vec3(-20.0f, 72.0f, 50));
		mModel = glm::rotate(mModel, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
		amModels[1].RenderModel(GL_POINTS);

		glBindVertexArray(uiVAOSceneObjects);

		FOR(i, 2)
		{
			glm::vec3 vCenter = glm::vec3(-40+i*40, 30, -20);
			mModel = glm::translate(glm::mat4(1.0), vCenter);
			if(i == 0)mModel = glm::rotate(mModel, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
			glDrawArrays(GL_POINTS, 0, iTorusFaces*3);

			mModel = glm::translate(glm::mat4(1.0), vCenter+glm::vec3(0.01f, 0.0f, 0.0f));
			if(i == 0)mModel = glm::rotate(mModel, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			mModel = glm::rotate(mModel, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
			glDrawArrays(GL_POINTS, 0, iTorusFaces*3);

			mModel = glm::translate(glm::mat4(1.0), vCenter+glm::vec3(0.00f, 0.01f, 0.0f));

			if(i == 0)mModel = glm::rotate(mModel, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			mModel = glm::rotate(mModel, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
			glDrawArrays(GL_POINTS, 0, iTorusFaces*3);
		}

		spNormalDisplayer.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", hmWorld.GetScaleMatrix());
		hmWorld.RenderHeightmapForNormals();
	}

	cCamera.Update();

	// PrGLint something over scene
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	GLint w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.PrintFormatted(20, h-30, 20, "FPS: %d", oglControl->GetFPS());
	ftFont.PrintFormatted(20, h-80, 20, "Heightmap size: %dx%d", hmWorld.GetNumHeightmapRows(), hmWorld.GetNumHeightmapCols());

	ftFont.PrintFormatted(20, h-110, 20, "Specular Intensity: %.2f (Press 'Q' and 'E' to change)", matShiny.fSpecularIntensity);
	if(Keys::Key('Q'))matShiny.fSpecularIntensity -= appMain.sof(0.2f);
	if(Keys::Key('E'))matShiny.fSpecularIntensity += appMain.sof(0.2f);

	ftFont.PrintFormatted(20, h-140, 20, "Specular Power: %.2f (Press 'Z' and 'C' to change)", matShiny.fSpecularPower);
	if(Keys::Key('Z'))matShiny.fSpecularPower -= appMain.sof(8.0f);
	if(Keys::Key('C'))matShiny.fSpecularPower += appMain.sof(8.0f);

	ftFont.PrintFormatted(20, h-200, 20, "Displaying Normals: %s (Press 'N' to toggle)", bDisplayNormals ? "Yes" : "Nah");
	if(Keys::Onekey('N'))bDisplayNormals = !bDisplayNormals;

	glEnable(GL_DEPTH_TEST);	
	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
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
}