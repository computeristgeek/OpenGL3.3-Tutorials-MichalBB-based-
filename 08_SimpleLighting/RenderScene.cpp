#include "common_header.h"

#include "Lin_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "walkingCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* One VBO, where all static data are stored now,
in this tutorial vertex is stored as 3 floats for
position, 2 floats for texture coordinate and
3 floats for normal vector. */

CVertexBufferObject vboSceneObjects;
GLuint uiVAOs[1]; // Only one VAO now

CShader shVertex, shFragment;
CShaderProgram spDirectionalLight;

CTexture tTextures[4];

CWalkingCamera cCamera;

/*---------------------------------------------*/

/*-----------------------------------------------

Name:	InitScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Initializes OpenGL features that will
		be used.

/*---------------------------------------------*/

#include "static_geometry.h"

GLint iTorusFaces1, iTorusFaces2;

GLvoid InitScene(GLvoid* lpParam)
{
	// For now, we just clear color to light blue,
	// to see if OpenGL context is working
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
		vCubeTexCoords[i] *= 10.0f;
		vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
		glm::vec3 vGroundNormal(0.0f, 1.0f, 0.0f);
		vboSceneObjects.addData(&vGroundNormal, sizeof(glm::vec3));
	}

	// Add torus to VBO and remember number of faces (triangles) of this torus
	iTorusFaces1 = generateTorus(vboSceneObjects, 7.0f, 2.0f, 20, 20);
	// Add sun torus to VBO
	iTorusFaces2 = generateTorus(vboSceneObjects, 10.0f, 6.0f, 10, 10);

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

	shVertex.LoadShader("data/shaders/shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("data/shaders/shader.frag", GL_FRAGMENT_SHADER);
	
	spDirectionalLight.CreateProgram();
	spDirectionalLight.AddShaderToProgram(&shVertex);
	spDirectionalLight.AddShaderToProgram(&shFragment);
	spDirectionalLight.LinkProgram();

	// Load textures

	string sTextureNames[] = {"ground.jpg", "box.jpg", "rust.jpg", "sun.jpg"};

	FOR(i, 4)
	{
		tTextures[i].loadTexture2D("data/textures/"+sTextureNames[i], true);
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	
	cCamera = CWalkingCamera(glm::vec3(0.0f, 3.0f, -20.0f), glm::vec3(0.0f, 3.0f, -19.0f), glm::vec3(0.0f, 1.0f, 0.0f), 35.0f);
}

/*-----------------------------------------------

Name:	RenderScene

Params:	lpParam - Pointer to OpenGL Control

Result:	Renders whole scene.

/*---------------------------------------------*/

GLfloat fGlobalAngle;
GLfloat fSunAngle = 45.0f;

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	oglControl->MakeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	spDirectionalLight.UseProgram();
	glBindVertexArray(uiVAOs[0]);

	// Set light properties

	GLfloat fSine = sin(fSunAngle*3.1415/180.0);
	glm::vec3 vSunPos(cos(fSunAngle*3.1415/180.0)*70, sin(fSunAngle*3.1415/180.0)*70, 0.0);

	// We'll change color of skies depending on sun's position
	glClearColor(0.0f, max(0.0f, 0.9f*fSine), max(0.0f, 0.9f*fSine), 1.0f);

	spDirectionalLight.setUniform("sunLight.vColor", glm::vec3(1.0f, 1.0f, 1.0f));
	spDirectionalLight.setUniform("sunLight.fAmbientIntensity", 0.25f);
	spDirectionalLight.setUniform("sunLight.vDirection", -glm::normalize(vSunPos));

	spDirectionalLight.setUniform("projectionMatrix", oglControl->getProjectionMatrix());

	glm::mat4 mModelView = cCamera.look();
	glm::mat4 mModelToCamera;

	spDirectionalLight.setUniform("gSampler", 0);

	spDirectionalLight.setUniform("modelViewMatrix", &mModelView);
	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Render ground

	tTextures[0].bindTexture();
	glDrawArrays(GL_TRIANGLES, 36, 6);

	tTextures[1].bindTexture();

	FOR(i, 5)
	{
		GLfloat fSign = -1.0f+float(i%2)*2.0f; // This just returns -1.0f or 1.0f (try to examine this)
		glm::vec3 vPos = glm::vec3(fSign*15.0f, 0.0f, 50.0f-float(i)*25.0f);
		mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
		mModelToCamera = glm::scale(mModelToCamera, glm::vec3(8.0f, 8.0f, 8.0f));
		// We need to trasnsform normals properly, it's done by transpose of inverse matrix of rotations and scales
		spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
		spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Render 5 tori (plural of torus :D)

	tTextures[2].bindTexture();

	FOR(i, 5)
	{
		GLfloat fSign = 1.0f-float(i%2)*2.0f; // This just returns -1.0f or 1.0f (try to examine this)
		glm::vec3 vPos = glm::vec3(fSign*15.0f, 0.0f, 50.0f-float(i)*25.0f);
		mModelToCamera = glm::translate(glm::mat4(1.0), vPos);
		mModelToCamera = glm::rotate(mModelToCamera, fGlobalAngle+i*30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));
		spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
		glDrawArrays(GL_TRIANGLES, 42, iTorusFaces1*3);
	}

	tTextures[3].bindTexture();

	// Render "sun" :D,

	mModelToCamera = glm::translate(glm::mat4(1.0), vSunPos);
	spDirectionalLight.setUniform("modelViewMatrix", mModelView*mModelToCamera);
	spDirectionalLight.setUniform("normalMatrix", glm::transpose(glm::inverse(mModelToCamera)));

	// It must shine
	spDirectionalLight.setUniform("sunLight.fAmbientIntensity", 0.8f);

	glDrawArrays(GL_TRIANGLES, 42+iTorusFaces1*3, iTorusFaces2*3);

	fGlobalAngle += appMain.sof(100.0f);
	cCamera.update();

	oglControl->SwapBuffersM();
}

/*-----------------------------------------------

Name:	ReleaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
{
	FOR(i, 4)tTextures[i].releaseTexture();

	spDirectionalLight.DeleteProgram();

	shFragment.DeleteShader();
	shVertex.DeleteShader();

	glDeleteVertexArrays(1, uiVAOs);
	vboSceneObjects.releaseVBO();
}

/*-----------------------------------------------

Name:	key_CB

Params:	[in]	window	The window that received the event.
	[in]	key	The keyboard key that was pressed or released.
	[in]	scancode	The system-specific scancode of the key.
	[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
	[in]	mods	Bit field describing which modifier keys were held down

Result:	Keyboard Callback

/*---------------------------------------------*/

GLvoid key_CB(GLFWwindow* hWnd, int key, int scancode, int action, int mods)
{
	glfwMakeContextCurrent(hWnd);
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			cout<<"Normal Exit:ESC Pressed"<<endl;
			glfwSetWindowShouldClose(hWnd, GL_TRUE);
			break;
		case 'C':
			if(action==GLFW_PRESS && mods==GLFW_MOD_CONTROL)
			{
				cout<<"Normal Exit:^C Pressed"<<endl;
				glfwSetWindowShouldClose(hWnd, GL_TRUE);
			}
			break;
		case 'W':
			cCamera.move(appMain.sof(cCamera.getfSpeed()));
			break;
		case 'S':
			cCamera.move(appMain.sof(-cCamera.getfSpeed()));
			break;
		case 'A':
			cCamera.rotateViewY(appMain.sof(90.0f));
			break;
		case 'D':
			cCamera.rotateViewY(appMain.sof(-90.0f));
			break;
		case GLFW_KEY_LEFT:
			fSunAngle -= appMain.sof(45.0f);
			break;
		case GLFW_KEY_RIGHT:
			fSunAngle += appMain.sof(45.0f);
			break;
	}
}

/*-----------------------------------------------

Name:	framebuffer_CB

Params:	[in]	window	The window whose framebuffer was resized.
	[in]	width	The new width, in pixels, of the framebuffer.
	[in]	height	The new height, in pixels, of the framebuffer.

Result:	Frame Buffer Size Callback

/*---------------------------------------------*/

GLvoid framebuffer_CB(GLFWwindow* hWnd, int width, int height)
{
	glfwMakeContextCurrent(hWnd);
	appMain.oglControl.ResizeOpenGLViewportFull();
	appMain.oglControl.setProjection3D(45.0f, float(width)/float(height), 0.001f, 1000.0f);
}

/*-----------------------------------------------

Name:	error_CB

Params:	[in]	error	An error code.
	[in]	description	A UTF-8 encoded string describing the error.

Result:	Error Callback

/*---------------------------------------------*/

void error_CB(int error, const char* description)
{
	cerr<<"Error "<<hex<<error<<":"<<description<<endl;
}