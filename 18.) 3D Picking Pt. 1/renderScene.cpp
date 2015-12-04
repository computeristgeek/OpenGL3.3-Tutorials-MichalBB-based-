#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"

#include "spotLight.h"
#include "dirLight.h"

#include "framebuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "picking.h"

#define NUMTEXTURES 1

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
UINT uiVAOs[1]; // Only one VAO now

CFreeTypeFont ftFont;

CTexture tTextures[NUMTEXTURES];
CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;
CSpotLight slFlashLight;

#include "objModel.h"

CObjModel mdlAlien;
CObjModel mdlSpongeBob;
CObjModel mdlThor;

CFramebuffer fboPickBuffer;

/*-----------------------------------------------

Name:    InitScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
         be used.

/*---------------------------------------------*/

#include "static_geometry.h"

void InitScene(LPVOID lpParam)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Prepare all scene objects

	vboSceneObjects.CreateVBO();
	glGenVertexArrays(1, uiVAOs); // Create one VAO
	glBindVertexArray(uiVAOs[0]);

	vboSceneObjects.BindVBO();

	AddSceneObjects(vboSceneObjects);

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


	if(!PrepareShaderPrograms())
	{
		PostQuitMessage(0);
		return;
	}
	// Load textures

	string sTextureNames[] = {"grass.png"};

	FOR(i, NUMTEXTURES)
	{
		tTextures[i].LoadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we print with size above 32, the quality will be low
	ftFont.LoadSystemFont("arial.ttf", 32);
	ftFont.SetShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 10.0f, 120.0f), glm::vec3(0.0f, 10.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\jajdarkland1\\", "jajdarkland1_ft.jpg", "jajdarkland1_bk.jpg", "jajdarkland1_lf.jpg", "jajdarkland1_rt.jpg", "jajdarkland1_up.jpg", "jajdarkland1_dn.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f);
	// NEW: Creating spotlight, position and direction will get updated every frame, that's why zero vectors
	slFlashLight = CSpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1, 15.0f, 0.017f);

	mdlAlien.LoadModel("data\\models\\alien2_obj\\alien2.obj", "alien2.mtl");
	mdlSpongeBob.LoadModel("data\\models\\Spongebob\\spongebob_bind.obj", "spongebob_bind.mtl");
	mdlThor.LoadModel("data\\models\\Thor\\thor.obj", "thor.mtl");
}



/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

void RenderBoundingBoxes(glm::mat4* mProj, bool bRedBox)
{
	spColor.UseProgram();
	spColor.SetUniform("matrices.projectionMatrix", mProj);

	glm::vec4 vRed(1.0f, 0.0f, 0.0f, 1.0f); // Red color

	glm::mat4 mView = cCamera.Look();
	spColor.SetUniform("matrices.viewMatrix", &mView);

	// Render SpongeBob :D

	glm::mat4 mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-40.0f, 0, -30.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	spColor.SetUniform("matrices.modelMatrix", &mModelMatrix);
	
	spColor.SetUniform("vColor", bRedBox ? vRed : GetColorByIndex(0));
	mdlSpongeBob.RenderBoundingBox();

	// Render strange creature found on internet as free model :D

	mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -30.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	spColor.SetUniform("matrices.modelMatrix", &mModelMatrix);
	
	spColor.SetUniform("vColor", bRedBox ? vRed : GetColorByIndex(1));
	mdlAlien.RenderBoundingBox();

	// Render Thor

	mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(40.0f, 0.0f, -30.0f));
	mModelMatrix = glm::rotate(mModelMatrix, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	spColor.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spColor.SetUniform("matrices.modelMatrix", &mModelMatrix);

	spColor.SetUniform("vColor", bRedBox ? vRed : GetColorByIndex(2));
	mdlThor.RenderBoundingBox();
}

float fGlobalAngle;
bool bWireFrame = false;
bool bShowBB = false;
int iSelectedIndex = -1;

string sObjectNames[] = 
{
	"SpongeBob",
	"Alien Creature",
	"Thor"
};

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

void RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(Keys::Onekey(VK_LBUTTON))
	{
		RenderBoundingBoxes(oglControl->GetProjectionMatrix(), false);
		iSelectedIndex = GetPickedColorIndexUnderMouse();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	spMain.UseProgram();

	glm::mat4 mModelMatrix, mView;

	// NEW: Set spotlight parameters

	glm::vec3 vSpotLightPos = cCamera.vEye;
	glm::vec3 vCameraDir = glm::normalize(cCamera.vView-cCamera.vEye);
	// Move down a little
	vSpotLightPos.y -= 3.2f;
	// Find direction of spotlight
	glm::vec3 vSpotLightDir = (vSpotLightPos+vCameraDir*75.0f)-vSpotLightPos;
	vSpotLightDir = glm::normalize(vSpotLightDir);
	// Find vector of horizontal offset
	glm::vec3 vHorVector = glm::cross(cCamera.vView-cCamera.vEye, cCamera.vUp);
	vSpotLightPos += vHorVector*3.3f;
	// Set it
	slFlashLight.vPosition = vSpotLightPos;
	slFlashLight.vDirection = vSpotLightDir;
	
	slFlashLight.SetUniformData(&spMain, "spotLight");

	oglControl->ResizeOpenGLViewportFull();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	spMain.UseProgram();

	spMain.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spMain.SetUniform("gSampler", 0);

	mView = cCamera.Look();
	spMain.SetUniform("matrices.viewMatrix", &mView);

	mModelMatrix = glm::translate(glm::mat4(1.0f), cCamera.vEye);
	
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mView*mModelMatrix)));

	CDirectionalLight dlSun2 = dlSun;

	// We set full ambient for skybox, so that its color isn't affected by directional light

	dlSun2.fAmbient = 1.0f;
	dlSun2.vColor = glm::vec3(0.1f, 0.1f, 0.1f);
	dlSun2.SetUniformData(&spMain, "sunLight");

	sbMainSkybox.RenderSkybox();

	glBindVertexArray(uiVAOs[0]);

	dlSun.SetUniformData(&spMain, "sunLight");

	CDirectionalLight dl2 = dlSun;
	dl2.fAmbient = 1.0f;
	dl2.SetUniformData(&spMain, "sunLight");
	
	if(bWireFrame)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	spMain.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0f));

	// Render ground

	tTextures[0].BindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Render SpongeBob :D

	mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-40.0f, 0, -30.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlSpongeBob.RenderModel();

	// Render strange creature found on internet as free model :D

	mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -30.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlAlien.RenderModel();

	// Render Thor

	mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(40.0f, 0.0f, -30.0f));
	mModelMatrix = glm::rotate(mModelMatrix, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlThor.RenderModel();

	// Render bounding boxes

	if(bShowBB)
		RenderBoundingBoxes(oglControl->GetProjectionMatrix(), true);

	cCamera.Update();

	// Print something over scene
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	int w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.PrintFormatted(20, h-30, 20, "Polygon Mode: %s (Press Q to toggle)", bWireFrame ? "Wireframe" : "Normal");
	ftFont.PrintFormatted(20, h-55, 20, "FPS: %d", oglControl->GetFPS());
	ftFont.PrintFormatted(20, h-80, 20, "Show bounding boxes: %s (Press 'B' to toggle)", bShowBB ? "On" : "Off");
	ftFont.PrintFormatted(20, h-105, 20, "Last object: %s", iSelectedIndex == -1 ? "none" : sObjectNames[iSelectedIndex].c_str());
	ftFont.PrintFormatted(20, h-130, 20, "Flashlight: %s (Press 'F' to toggle)", slFlashLight.bOn ? "On" : "Off");

	if(Keys::Onekey('Q'))
		bWireFrame = !bWireFrame;
	if(Keys::Onekey('F'))
		slFlashLight.bOn = 1-slFlashLight.bOn;
	if(Keys::Onekey('B'))
		bShowBB = !bShowBB;

	glEnable(GL_DEPTH_TEST);
	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

void ReleaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	sbMainSkybox.DeleteSkybox();

	spMain.DeleteProgram();
	spOrtho2D.DeleteProgram();
	spFont2D.DeleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].DeleteShader();
	ftFont.DeleteFont();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.DeleteVBO();
	mdlAlien.DeleteModel();
	mdlSpongeBob.DeleteModel();
}