#include "common_header.h"

#include "win_OpenGLApp.h"

/*-----------------------------------------------

Name:	InitScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
		be used.

/*---------------------------------------------*/

GLfloat fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
GLfloat fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)

GLuint uiVBO[2];

GLvoid InitScene(GLvoid* lpParam)
{
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	// Setup triangle vertices
	fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
	fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
	fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;
 
	// Setup quad vertices
 
	fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
	fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
	fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
	fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

	// Now we create two VBOs
	glGenBuffers(2, uiVBO);
 
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
}

/*-----------------------------------------------

Name:	RenderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	// We just clear color
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	// Triangle render
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Quad render using triangle strip
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	oglControl->SwapBuffersM();
}
