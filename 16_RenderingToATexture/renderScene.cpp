#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"

#include "dirLight.h"

#include "framebuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUMSHADERS 11
#define NUMTEXTURES 3

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
GLuint uiVAOs[1]; // Only one VAO now

CShader shShaders[NUMSHADERS];
CShaderProgram spMain, spOrtho2D, spFont2D, spColor, spMainFB;

CFreeTypeFont ftFont;

CTexture tTextures[NUMTEXTURES];
CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;

#include "objModel.h"

CObjModel mdlThor;
CObjModel mdlSpongeBob;

CFramebuffer fboTelevision;

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

	// Add cube to VBO

	FOR(i, 36)
	{
		vboSceneObjects.AddData(&vCubeVertices[i], sizeof(glm::vec3));
		vboSceneObjects.AddData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
		vboSceneObjects.AddData(&vCubeNormals[i/6], sizeof(glm::vec3));
	}

	// Add ground to VBO

	FOR(i, 6)
	{
		vboSceneObjects.AddData(&vGround[i], sizeof(glm::vec3));
		glm::vec2 vCoord = vCubeTexCoords[i] * 50.0f;
		vboSceneObjects.AddData(&vCoord, sizeof(glm::vec2));
		glm::vec3 vGroundNormal(0.0f, 1.0f, 0.0f);
		vboSceneObjects.AddData(&vGroundNormal, sizeof(glm::vec3));
	}

	// Add building to VBO

	FOR(i, 24)
	{
		vboSceneObjects.AddData(&vBuilding[i], sizeof(glm::vec3));
		glm::vec2 vCoord = vCubeTexCoords[i%6]*10.0f;
		vboSceneObjects.AddData(&vCoord, sizeof(glm::vec2));
		vboSceneObjects.AddData(&vBuildingNormals[i/6], sizeof(glm::vec3));
	}

	// Add cinema to VBO

	FOR(i, 4)
	{
		vboSceneObjects.AddData(&vCinema[i], sizeof(glm::vec3));
		vboSceneObjects.AddData(&vCinemaCoords[i], sizeof(glm::vec2));
		glm::vec3 vNormal(0, 0, 1);
		vboSceneObjects.AddData(&vNormal, sizeof(glm::vec3));
	}

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

	// Load shaders and create shader program

	string sShaderFileNames[] = {"main_shader.vert", "main_shader.geom", "main_shader.frag", "ortho2D.vert",
		"ortho2D.frag", "font2D.frag", "dirLight.frag", "pointLight.frag", "fog.frag", "color.vert", "color.frag",
		};

	FOR(i, NUMSHADERS)
	{
		string sExt = sShaderFileNames[i].substr(ESZ(sShaderFileNames[i])-4, 4);
		GLint iShaderType = sExt == "vert" ? GL_VERTEX_SHADER : (sExt == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);
		shShaders[i].LoadShader("data\\shaders\\"+sShaderFileNames[i], iShaderType);
	}

	// Create shader programs

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shShaders[0]);
	spMain.AddShaderToProgram(&shShaders[1]);
	spMain.AddShaderToProgram(&shShaders[2]);
	spMain.AddShaderToProgram(&shShaders[6]);
	spMain.AddShaderToProgram(&shShaders[7]);
	spMain.LinkProgram();

	spOrtho2D.CreateProgram();
	spOrtho2D.AddShaderToProgram(&shShaders[3]);
	spOrtho2D.AddShaderToProgram(&shShaders[4]);
	spOrtho2D.LinkProgram();

	spFont2D.CreateProgram();
	spFont2D.AddShaderToProgram(&shShaders[3]);
	spFont2D.AddShaderToProgram(&shShaders[5]);
	spFont2D.LinkProgram();

	spColor.CreateProgram();
	spColor.AddShaderToProgram(&shShaders[9]);
	spColor.AddShaderToProgram(&shShaders[10]);
	spColor.LinkProgram();

	spMainFB.CreateProgram();
	spMainFB.AddShaderToProgram(&shShaders[11]);
	spMainFB.AddShaderToProgram(&shShaders[12]);
	spMainFB.AddShaderToProgram(&shShaders[13]);
	spMainFB.AddShaderToProgram(&shShaders[6]);
	spMainFB.AddShaderToProgram(&shShaders[7]);
	spMainFB.LinkProgram();

	// Load textures

	string sTextureNames[] = {"Tile41a.jpg", "met_wall01a.jpg", "floor.jpg"};

	FOR(i, NUMTEXTURES)
	{
		tTextures[i].LoadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.LoadSystemFont("arial.ttf", 32);
	ftFont.SetShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 10.0f, 120.0f), glm::vec3(0.0f, 10.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\jajdarkland1\\", "jajdarkland1_ft.jpg", "jajdarkland1_bk.jpg", "jajdarkland1_lf.jpg", "jajdarkland1_rt.jpg", "jajdarkland1_up.jpg", "jajdarkland1_dn.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f);

	mdlThor.LoadModel("data\\models\\Thor\\thor.obj", "thor.mtl");
	mdlSpongeBob.LoadModel("data\\models\\Spongebob\\spongebob_bind.obj", "spongebob_bind.mtl");

	fboTelevision.CreateFramebufferWithTexture(512, 256);
	fboTelevision.AddDepthBuffer();
	fboTelevision.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;
GLboolean bWireFrame = false;

GLvoid RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	// Render Thor and The Avengers text into framebuffer
	
	fboTelevision.BindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();
	spMain.SetUniform("matrices.projMatrix", fboTelevision.CalculateProjectionMatrix(45.0f, 0.5f, 1000.0f));
	dlSun.SetUniformData(&spMain, "sunLight");

	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 160.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	spMain.SetUniform("matrices.viewMatrix", &mView);

	spMain.SetUniform("gSampler", 0);
	spMain.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	static glm::vec3 vThorPosition = glm::vec3(0.0f, -40.0f, -30.0f);
	glm::mat4 mModelMatrix = glm::translate(glm::mat4(1.0),vThorPosition);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlThor.RenderModel();

	// Render ground for Thor

	glBindVertexArray(uiVAOs[0]);

	spMain.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spMain.SetUniform("matrices.modelMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -40.0f, 0.0f)));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0f));

	tTextures[2].BindTexture();
	glDrawArrays(GL_TRIANGLES, 36, 6);

	// PrGLint The Avengers in the center

	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", fboTelevision.CalculateOrthoMatrix());

	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	
	GLint iTextWidth = ftFont.GetTextWidth("The Avengers", 40);
	ftFont.Print("The Avengers", (fboTelevision.GetWidth()-iTextWidth)/2, fboTelevision.GetHeight()-60, 40);

	// Update Thor's position and rotation

	vThorPosition.x += appMain.sof(60.0f);
	if(vThorPosition.x > 160)vThorPosition.x = -160;
	fGlobalAngle += appMain.sof(170.0f);

	glEnable(GL_DEPTH_TEST);

	//******************************
	//******************************
	//******************************

	// Get back to normal rendering, no framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
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
	dlSun2.SetUniformData(&spMain, "sunLight");

	sbMainSkybox.RenderSkybox();

	glBindVertexArray(uiVAOs[0]);

	dlSun.SetUniformData(&spMain, "sunLight");
	
	if(bWireFrame)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	spMain.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0f));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0f));

	// Render ground

	tTextures[0].BindTexture();
	glDrawArrays(GL_TRIANGLES, 36, 6);
	
	// Render building

	tTextures[1].BindTexture();
	glDrawArrays(GL_TRIANGLES, 42, 24);

	// Render cinema

	fboTelevision.BindFramebufferTexture(0, true);
	glDrawArrays(GL_TRIANGLE_STRIP, 66, 4);

	// Render SpongeBob :D

	mModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(30, 0, 10));
	mModelMatrix = glm::rotate(mModelMatrix, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(20, 20, 20));
	spMain.SetUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.SetUniform("matrices.modelMatrix", &mModelMatrix);
	mdlSpongeBob.RenderModel();

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
	ftFont.PrintFormatted(20, h-80, 20, "Model polygon count (Thor): %d", mdlThor.GetPolygonCount());
	ftFont.PrintFormatted(20, h-105, 20, "Model polygon count (SpongeBob): %d", mdlSpongeBob.GetPolygonCount());
	if(Keys::Onekey('Q'))
	{
		bWireFrame = !bWireFrame;

	}

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
	mdlThor.DeleteModel();
	mdlSpongeBob.DeleteModel();

	fboTelevision.DeleteFramebuffer();
}