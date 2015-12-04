#include "mainheader.h"

CCamera camMain;
CWorld wWorld;
CFreeType ft;

//====================

void glInit()
{
	glEnable(GL_DEPTH_TEST); // Enables depth-testing
	glEnable(GL_TEXTURE_2D); // Enables texturing
	glShadeModel(GL_SMOOTH); // Enables smooth shading
	glDepthFunc(GL_LESS);	// The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	camMain = CCamera(CVector3(0, 3, 0), CVector3(0, 3, -1), CVector3(0, 1, 0), 20.0f, 20.0f);
	camMain.setMoving('W', 'S', 'A', 'D');
	loadTextures();
	wWorld.loadWorld("world.txt");
	ft.createFont("Font.ttf", 32);
	loadFireworksModels();

}

//====================

void glScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
	glLoadIdentity(); // Reset matrix
	camMain.look(); camMain.update();
	camMain.checkCollision();
	wWorld.renderWorld();
	renderAllParticles(pParts, 1);

	updateFireworks();

	SwapBuffers(glAp.hDC); // Swap drawing buffers
}

//====================

void freeEverything()
{
	while(ESZ(tTextures) > 0)
	{
		glDeleteTextures(1, &tTextures[0].second);
		tTextures.erase(tTextures.begin());
	}
}