#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*-----------------------------------------------

Name:	initScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
		be used.

/*---------------------------------------------*/

#define HM_SIZE_X 4 // Dimensions of our heightmap
#define HM_SIZE_Y 4

UINT uiVBOHeightmapData; // Here are stored heightmap data (vertices)
UINT uiVBOIndices; // And here indices for rendering heightmap

UINT uiVAOHeightmap; // One VAO for heightmap

GLboolean bShowFPS = false;
GLboolean bVerticalSync = true;

CShader shVertex, shFragment;
CShaderProgram spMain;

GLvoid initScene(LPVOID lpParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Setup heightmap

	glGenVertexArrays(1, &uiVAOHeightmap); // Create one VAO
	glGenBuffers(1, &uiVBOHeightmapData); // One VBO for data
	glGenBuffers(1, &uiVBOIndices); // And finally one VBO for indices

	glBindVertexArray(uiVAOHeightmap);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBOHeightmapData);

	glm::vec3 vHeightmapData[HM_SIZE_X*HM_SIZE_Y]; // Here the heightmap vertices will be stored temporarily

	GLfloat fHeights[HM_SIZE_X*HM_SIZE_Y] =
	{
		4.0f, 2.0f, 3.0f, 1.0f,
		3.0f, 5.0f, 8.0f, 2.0f,
		7.0f, 10.0f, 12.0f, 6.0f,
		4.0f, 6.0f, 8.0f, 3.0f
	};

	GLfloat fSizeX = 40.0f, fSizeZ = 40.0f;

	FOR(i, HM_SIZE_X*HM_SIZE_Y)
	{
		GLfloat column = float(i%HM_SIZE_X), row = float(i/HM_SIZE_X);
		vHeightmapData[i] = glm::vec3(
			-fSizeX/2+fSizeX*column/float(HM_SIZE_X-1), // X Coordinate
			fHeights[i],									// Y Coordinate (it's height)
			-fSizeZ/2+fSizeZ*row/float(HM_SIZE_Y-1)	// Z Coordinate
			);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*HM_SIZE_X*HM_SIZE_Y, vHeightmapData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &uiVBOIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBOIndices);
	GLint iIndices[] =
	{
		0, 4, 1, 5, 2, 6, 3, 7, 16, // First row, then restart
		4, 8, 5, 9, 6, 10, 7, 11, 16, // Second row, then restart
		8, 12, 9, 13, 10, 14, 11, 15, // Third row, no restart
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iIndices), iIndices, GL_STATIC_DRAW);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(HM_SIZE_X*HM_SIZE_Y);

	// Load shaders and create shader program

	shVertex.loadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shFragment.loadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spMain.createProgram();
	spMain.addShaderToProgram(&shVertex);
	spMain.addShaderToProgram(&shFragment);

	spMain.linkProgram();
	spMain.useProgram();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

/*-----------------------------------------------

Name:	renderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

GLfloat fRotationAngle = 0.0f;
const GLfloat PIover180 = 3.1415f/180.0f;

GLvoid renderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(uiVAOHeightmap);

	GLint iModelViewLoc = glGetUniformLocation(spMain.getProgramID(), "modelViewMatrix");
	GLint iProjectionLoc = glGetUniformLocation(spMain.getProgramID(), "projectionMatrix");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*oglControl->getProjectionMatrix()));

	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 60, 30), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mCurrent = glm::rotate(mModelView, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glBindVertexArray(uiVAOHeightmap);
	glDrawElements(GL_TRIANGLE_STRIP, HM_SIZE_X*(HM_SIZE_Y-1)*2+HM_SIZE_Y-2, GL_UNSIGNED_INT, 0);

	fRotationAngle += appMain.sof(30.0f);

	// If user presses escape, quit the application
	if(Keys::onekey(VK_ESCAPE))PostQuitMessage(0);

	oglControl->swapBuffers();
}

/*-----------------------------------------------

Name:	releaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid releaseScene(LPVOID lpParam)
{
	spMain.deleteProgram();

	glDeleteBuffers(1, &uiVBOHeightmapData);
	glDeleteBuffers(1, &uiVBOIndices);
	glDeleteVertexArrays(1, &uiVAOHeightmap);

	shVertex.deleteShader();
	shFragment.deleteShader();
}