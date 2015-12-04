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

#define NUMTEXTURES 2

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
GLuint uiVAOs[1]; // Only one VAO now

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

GLvoid InitScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	string sTextureNames[] = {"images.jpeg", "blackhole.jpg"};

	FOR(i, NUMTEXTURES)
	{
		tTextures[i].LoadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
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

glm::vec3 vObjectPositions[] =
{
	glm::vec3(-40.0f, 0, -30.0f),
	glm::vec3(0.0f, 0.0f, -30.0f),
	glm::vec3(40.0f, 0, -30.0f),
	glm::vec3(0.0f, 100.0f, -30.0f),
};

// Because some of these models aren't centered, we must offset bounding spheres a little

glm::vec3 vSpherePositions[] =
{
	vObjectPositions[0]+glm::vec3(0.0f, 9.0f, 0.0f),
	vObjectPositions[1]+glm::vec3(0.0f, 5.0f, -5.0f),
	vObjectPositions[2]+glm::vec3(0.0f, 15.0f, 0.0f),
	vObjectPositions[3]
};

GLfloat fSphereRadii[] = {11.0f, 9.2f, 10.0f, 40.0f};

/*-----------------------------------------------

Name:    RenderBoundingSpheres

Params:  mProj - Projection matrix used on scene (just for
				 shader, it should be passed nicer way but
				 what the heck).

Result:  Renders spheres surrounding our scene objects.

/*---------------------------------------------*/

GLvoid RenderBoundingSpheres(glm::mat4* mProj)
{
	glBindVertexArray(uiVAOs[0]);
	spColor.UseProgram();
	spColor.SetUniform("matrices.projectionMatrix", mProj);

	glm::vec4 vRed(1.0f, 0.0f, 0.0f, 1.0f); // Red color
	spColor.SetUniform("vColor", vRed);

	glm::mat4 mView = cCamera.Look();
	spColor.SetUniform("matrices.viewMatrix", &mView);

	FOR(i, 4)
	{
		glm::mat4 mModelMatrix = glm::translate(glm::mat4(1.0), vSpherePositions[i]);
		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(fSphereRadii[i], fSphereRadii[i], fSphereRadii[i]));
		spColor.SetUniform("matrices.modelMatrix", &mModelMatrix);

		glDrawArrays(GL_TRIANGLES, 6, iSphereFaces*3);
	}

}

GLfloat fGlobalAngle;
GLboolean bWireFrame = false;
GLboolean bShowBS = true;
GLint iSelectedIndex = -1;

string sObjectNames[] = 
{
	"SpongeBob",
	"Alien Creature",
	"Thor",
	"Black Hole",
};

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

GLvoid RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	// Render Black Hole

	mModelMatrix = glm::translate(glm::mat4(1.0), vObjectPositions[3]);
	// To make things interesting, let's rotate the black hole
	fGlobalAngle += appMain.sof(78.0f);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	// Lower the sphere radius a little so bounding sphere and actual sphere don't overlap when both rendered
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(fSphereRadii[3]-0.1f, fSphereRadii[3]-0.1f, fSphereRadii[3]-0.1f));

	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	tTextures[1].BindTexture();
	glDrawArrays(GL_TRIANGLES, 6, iSphereFaces*3);

	// Render SpongeBob :D

	mModelMatrix = glm::translate(glm::mat4(1.0), vObjectPositions[0]);
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlSpongeBob.RenderModel();

	// Render strange creature found on internet as free model :D

	mModelMatrix = glm::translate(glm::mat4(1.0), vObjectPositions[1]);
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlAlien.RenderModel();

	// Render Thor

	mModelMatrix = glm::translate(glm::mat4(1.0),vObjectPositions[2]);
	mModelMatrix = glm::rotate(mModelMatrix, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlThor.RenderModel();

	// Check for collisions if we left click the mouse button
	if(Keys::Onekey(VK_LBUTTON))
	{
		glm::vec3 v1, v2;
		Get3DRayUnderMouse(&v1, &v2);
		FOR(i, 4)
		{
			if(RaySphereCollision(vSpherePositions[i], fSphereRadii[i], v1, v2))
			{
				iSelectedIndex = i;
				break;
			}
		}
		GLint a = 5;
	}

	// Render bounding spheres

	if(bShowBS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		RenderBoundingSpheres(oglControl->GetProjectionMatrix());
	}

	cCamera.Update();

	// PrGLint something over scene
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	GLint w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.PrintFormatted(20, h-30, 20, "Polygon Mode: %s (Press Q to toggle)", bWireFrame ? "Wireframe" : "Normal");
	ftFont.PrintFormatted(20, h-55, 20, "FPS: %d", oglControl->GetFPS());
	ftFont.PrintFormatted(20, h-80, 20, "Show bounding spheres: %s (Press 'B' to toggle)", bShowBS ? "On" : "Off");
	ftFont.PrintFormatted(20, h-105, 20, "Last object: %s", iSelectedIndex == -1 ? "none" : sObjectNames[iSelectedIndex].c_str());
	ftFont.PrintFormatted(20, h-130, 20, "Flashlight: %s (Press 'F' to toggle)", slFlashLight.bOn ? "On" : "Off");

	if(Keys::Onekey('Q'))
		bWireFrame = !bWireFrame;
	if(Keys::Onekey('F'))
		slFlashLight.bOn = 1-slFlashLight.bOn;
	if(Keys::Onekey('B'))
		bShowBS = !bShowBS;

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

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.DeleteVBO();
	mdlAlien.DeleteModel();
	mdlSpongeBob.DeleteModel();
}