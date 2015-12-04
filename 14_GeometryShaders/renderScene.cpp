#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"

#include "dirLight.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUMSHADERS 11
#define NUMTEXTURES 4

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
UINT uiVAOs[1]; // Only one VAO now

CShader shShaders[NUMSHADERS];
CShaderProgram spMain, spOrtho2D, spFont2D, spColor;

CFreeTypeFont ftFont;

CTexture tTextures[NUMTEXTURES];
CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;

/*-----------------------------------------------

Name:    initScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
         be used.

/*---------------------------------------------*/

#include "static_geometry.h"

GLint iTorusFaces;

GLvoid initScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Prepare all scene objects

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

	string sShaderFileNames[] = {"main_shader.vert", "main_shader.geom", "main_shader.frag", "ortho2D.vert",
		"ortho2D.frag", "font2D.frag", "dirLight.frag", "pointLight.frag", "fog.frag", "color.vert", "color.frag"};

	FOR(i, NUMSHADERS)
	{
		string sExt = sShaderFileNames[i].substr(ESZ(sShaderFileNames[i])-4, 4);
		GLint iShaderType = sExt == "vert" ? GL_VERTEX_SHADER : (sExt == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);
		shShaders[i].loadShader("data\\shaders\\"+sShaderFileNames[i], iShaderType);
	}

	// Create shader programs

	spMain.createProgram();
	spMain.addShaderToProgram(&shShaders[0]);
	spMain.addShaderToProgram(&shShaders[1]);
	spMain.addShaderToProgram(&shShaders[2]);
	spMain.addShaderToProgram(&shShaders[6]);
	spMain.addShaderToProgram(&shShaders[7]);
	spMain.linkProgram();

	spOrtho2D.createProgram();
	spOrtho2D.addShaderToProgram(&shShaders[3]);
	spOrtho2D.addShaderToProgram(&shShaders[4]);
	spOrtho2D.linkProgram();

	spFont2D.createProgram();
	spFont2D.addShaderToProgram(&shShaders[3]);
	spFont2D.addShaderToProgram(&shShaders[5]);
	spFont2D.linkProgram();

	spColor.createProgram();
	spColor.addShaderToProgram(&shShaders[9]);
	spColor.addShaderToProgram(&shShaders[10]);
	spColor.linkProgram();

	// Load textures

	string sTextureNames[] = {"Tile41a.jpg", "crate.jpg", "metalplate.jpg", "stone_wall.jpg"};

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

	sbMainSkybox.loadSkybox("data\\skyboxes\\jajsundown1\\", "jajsundown1_ft.jpg", "jajsundown1_bk.jpg", "jajsundown1_lf.jpg", "jajsundown1_rt.jpg", "jajsundown1_up.jpg", "jajsundown1_dn.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.5f);
}

/*-----------------------------------------------

Name:    renderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;
GLfloat fBender = 0.75f;
GLboolean bWireFrame = false;

GLvoid renderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	spMain.useProgram();

	spMain.setUniform("fBender", fBender);
	if(Keys::key('X'))fBender += appMain.sof(2.0f);
	if(Keys::key('Z'))fBender -= appMain.sof(2.0f);

	spMain.setUniform("matrices.projMatrix", oglControl->getProjectionMatrix());
	spMain.setUniform("gSampler", 0);

	glm::mat4 mView = cCamera.look();
	spMain.setUniform("matrices.viewMatrix", &mView);
	spMain.setUniform("matrices.modelMatrix", glm::mat4(1.0f));

	glm::mat4 mModelMatrix = glm::translate(glm::mat4(1.0f), cCamera.vEye);
	
	spMain.setUniform("matrices.modelMatrix", &mModelMatrix);
	spMain.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mView*mModelMatrix)));

	CDirectionalLight dlSun2 = dlSun;

	// We set full ambient for skybox, so that its color isn't affected by directional light

	dlSun2.fAmbient = 1.0f;
	dlSun2.setUniformData(&spMain, "sunLight");

	sbMainSkybox.renderSkybox();

	glBindVertexArray(uiVAOs[0]);

	dlSun.setUniformData(&spMain, "sunLight");
	
	// Render ground

	if(bWireFrame)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	spMain.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spMain.setUniform("matrices.modelMatrix", glm::mat4(1.0f));
	spMain.setUniform("matrices.normalMatrix", glm::mat4(1.0f));
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
			spMain.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
			spMain.setUniform("matrices.modelMatrix", mModelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	// Render 3 rotated tori to create interesting object

	tTextures[2].bindTexture();
	
	// Now it's gonna GLfloat in the air
	glm::vec3 vPos = glm::vec3(0.0f, 50.0, 0.0f);
	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	spMain.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.setUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 66, iTorusFaces*3);

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	spMain.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.setUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 66, iTorusFaces*3);

	mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix, fGlobalAngle+90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	spMain.setUniform("matrices.normalMatrix", glm::transpose(glm::inverse(mModelMatrix)));
	spMain.setUniform("matrices.modelMatrix", &mModelMatrix);
	glDrawArrays(GL_TRIANGLES, 66, iTorusFaces*3);

	fGlobalAngle += appMain.sof(100.0f);
	cCamera.update();

	// PrGLint something over scene
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	spFont2D.useProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.setUniform("matrices.projMatrix", oglControl->getOrthoMatrix());

	GLint w = oglControl->getViewportWidth(), h = oglControl->getViewportHeight();
	
	spFont2D.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.printFormatted(20, h-30, 20, "Bender Value: %.3f  (Hold Z and X to change)", fBender);
	ftFont.printFormatted(20, h-55, 20, "Polygon Mode: %s (Press Q to toggle)", bWireFrame ? "Wireframe" : "Normal");

	if(Keys::onekey('Q'))
	{
		bWireFrame = !bWireFrame;

	}

	glEnable(GL_DEPTH_TEST);
	if(Keys::onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->swapBuffers();
}

/*-----------------------------------------------

Name:    releaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid releaseScene(LPVOID lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].releaseTexture();
	sbMainSkybox.releaseSkybox();

	spMain.deleteProgram();
	spOrtho2D.deleteProgram();
	spFont2D.deleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].deleteShader();
	ftFont.releaseFont();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.releaseVBO();
}