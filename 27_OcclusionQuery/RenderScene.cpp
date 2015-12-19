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

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;
CMaterial matShiny;

GLuint uiOcclusionQuery;

GLboolean bDisplayNormals = false; // Do not display normals by default

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

	PrepareStaticSceneObjects();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.LoadSystemFont("arial.ttf", 32);
	ftFont.SetShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 30.0f, 99.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\calm\\", "calm_front.jpg", "calm_back.jpg", "calm_right.jpg", "calm_left.jpg", "calm_top.jpg", "calm_top.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f, 0);
	
	matShiny = CMaterial(1.0f, 32.0f);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Occlusion query object
	glGenQueries(1, &uiOcclusionQuery);
}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

glm::vec3 vModelPosition = glm::vec3(0, 20, 0);
GLfloat fGlobalAngle;

GLboolean bShowOccluders = false;
GLboolean bEnableOcclusionQuery = true;

GLvoid RenderScene(GLvoid* lpParam)
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
	static GLfloat fAngleOfDarkness = 45.0f;
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

	// Render ground

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	tTextures[0].BindTexture();
	glDrawArrays(GL_TRIANGLES, iSphereFaces*3+36, 6);

	// Render that grid of where spheres are

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::translate(glm::mat4(1.0), glm::vec3(0.0f, fCubeHalfSize, 0.0f)));
	tTextures[1].BindTexture();
	glDrawArrays(GL_TRIANGLES, iSphereFaces*3, 36);

	GLint iSpheresPassed = 0;
	GLboolean bRenderSphere[3][3][3];
	glm::mat4 mModelMatrices[3][3][3];

	spOccluders.UseProgram();
	spOccluders.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spOccluders.SetUniform("matrices.viewMatrix", cCamera.Look());
	spOccluders.SetUniform("vColor", glm::vec4(1, 0, 0, 0));
	glBindVertexArray(uiVAOOccluders);

	// Occlusion query begins here
	// First of all, disable writing to the color buffer and depth buffer. We just wanna check if they would be rendered, not actually render them
	glColorMask(false, false, false, false);
	glDepthMask(GL_FALSE);

	FOR(x, 3)
	{
		FOR(y, 3)
		{
			FOR(z, 3)
			{
				bRenderSphere[x][y][z] = false;
				GLfloat fLocalRotAngle = fGlobalAngle + x*60.0f + y*20.0f + z*6.0f;
				glm::vec3 vOcclusionCubePos = glm::vec3(-fCubeHalfSize+fCubeHalfSize*x*2.0f/3.0f + fCubeHalfSize/3.0f, -fCubeHalfSize+fCubeHalfSize*y*2.0f/3.0f + fCubeHalfSize/3.0f, -fCubeHalfSize+fCubeHalfSize*z*2.0f/3.0f + fCubeHalfSize/3.0f);

				mModelMatrices[x][y][z] = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, fCubeHalfSize, 0.0f));
				mModelMatrices[x][y][z] = glm::translate(mModelMatrices[x][y][z], vOcclusionCubePos);
				mModelMatrices[x][y][z] = glm::rotate(mModelMatrices[x][y][z], fLocalRotAngle, glm::vec3(1, 0, 0));
				mModelMatrices[x][y][z] = glm::rotate(mModelMatrices[x][y][z], fLocalRotAngle, glm::vec3(0, 1, 0));
				mModelMatrices[x][y][z] = glm::rotate(mModelMatrices[x][y][z], fLocalRotAngle, glm::vec3(0, 0, 1));

				if(bEnableOcclusionQuery)
				{
					mModel = glm::scale(mModelMatrices[x][y][z], glm::vec3(fCubeHalfSize/3, fCubeHalfSize/3, fCubeHalfSize/3));
					spOccluders.SetUniform("matrices.modelMatrix", mModel);

					// Begin occlusion query
					glBeginQuery(GL_SAMPLES_PASSED, uiOcclusionQuery);
						// Every pixel that passes the depth test now gets added to the result
						glDrawArrays(GL_TRIANGLES, 0, 36);
					glEndQuery(GL_SAMPLES_PASSED);
					// Now get tthe number of pixels passed
					GLint iSamplesPassed = 0;
					glGetQueryObjectiv(uiOcclusionQuery, GL_QUERY_RESULT, &iSamplesPassed);
					
					// If some samples passed, this means, that we should better render the whole sphere, because we were able 
					// to see its bounding box
					if(iSamplesPassed > 0)
					{
						bRenderSphere[x][y][z] = true;
						// Increase the number of spheres that have passed
						iSpheresPassed++;
					}
				}
				else // If we do not use occlusion query, then all of the spheres have passed
				{
					bRenderSphere[x][y][z] = true;
					// Increase the number of spheres that have passed
					iSpheresPassed++;
				}
			}
		}
	}

	// Re-enable writing to color buffer and depth buffer
	glColorMask(true, true, true, true);
	glDepthMask(GL_TRUE);

	// Show either occluders or spheres
	if(bShowOccluders)
	{
		FOR(x, 3)
		{
			FOR(y, 3)
			{
				FOR(z, 3)
				{
					mModel = glm::scale(mModelMatrices[x][y][z], glm::vec3(fCubeHalfSize/3, fCubeHalfSize/3, fCubeHalfSize/3));
					spOccluders.SetUniform("matrices.modelMatrix", mModel);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}
	}
	else
	{
		spMain.UseProgram();
		glBindVertexArray(uiVAOSceneObjects);
		tTextures[2].BindTexture();
		FOR(x, 3)
		{
			FOR(y, 3)
			{
				FOR(z, 3)
				{
					if(bRenderSphere[x][y][z] == false)
						continue;
					spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelMatrices[x][y][z]);
					glDrawArrays(GL_TRIANGLES, 0, iSphereFaces*3);
				}
			}
		}
	}
	fGlobalAngle += appMain.sof(45.0f);

	cCamera.Update();

	// PrGLint something over scene
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	GLint w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);
	ftFont.PrintFormatted(20, h-30, 20, "FPS: %d", oglControl->GetFPS());

	ftFont.PrintFormatted(20, h-80, 20, "Occlusion Query: %s ('X' to toggle)", bEnableOcclusionQuery ? "Enabled" : "Disabled");
	ftFont.PrintFormatted(20, h-105, 20, "Show Occluders: %s ('C' to toggle)", bShowOccluders ? "Yes Please" : "No");
	ftFont.PrintFormatted(20, h-130, 20, "Spheres Rendered: %d / 27", iSpheresPassed);
	ftFont.PrintFormatted(20, h-155, 20, "Faces Of One Sphere: %d", iSphereFaces);

	if(Keys::Onekey('X'))bEnableOcclusionQuery = !bEnableOcclusionQuery;
	if(Keys::Onekey('C'))bShowOccluders = !bShowOccluders;

	glEnable(GL_DEPTH_TEST);	
	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to OpenGL control.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	sbMainSkybox.DeleteSkybox();

	DeleteAllShaderPrograms();

	ftFont.DeleteFont();

	glDeleteVertexArrays(1, &uiVAOSceneObjects);
	vboSceneObjects.DeleteVBO();

	glDeleteQueries(1, &uiOcclusionQuery);
}