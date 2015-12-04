#include "common_header.h"

#include "win_OpenGLApp.h"
#include "texture.h"
#include "flyingCamera.h"
#include "freeTypeFont.h"
#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../include/blaze_part_sys_engine.h"
#include "../include/blaze_effects.h"

#include "demoScenes.h"

void prepareFireDemoScene()
{
	BlazeEffects::setParticleSystemEngine(psEngine);

	BlazeEffects::createFire((float*)glm::value_ptr(glm::vec3(148, 0.0f, -148.0f)), (float*)glm::value_ptr(glm::vec3(4.0f, 0.0f, 4.0f)), 200, 64, 0);

	BlazeEffects::createFireRing((float*)glm::value_ptr(glm::vec3(-50.0f, 0.0f, 50.0f)), 20.0f, 20, 51, 189, 255);
	BlazeEffects::createFireRing((float*)glm::value_ptr(glm::vec3(50.0f, 0.0f, -150.0f)), 30.0f, 30, 255, 128, 0);
	BlazeEffects::createFireRing((float*)glm::value_ptr(glm::vec3(-150.0f, 0.0f, -150.0f)), 10.0f, 15, 255, 0, 0);

	BlazeEffects::createFireGrid(glm::vec3(0.0f, 28.0f, 120.0f), 50.0f, 4, 255, 128, 0);

	psEngine->setParticleTexture(tFireTexture.getTextureID());
}

void renderWall(glm::vec3 vCorner, glm::vec3 vDir, float fLength, float fWidth, float fHeight, UINT uiTex)
{
	vDir = glm::normalize(vDir);
	tWorldTextures[uiTex].bindTexture();
	glm::vec3 vPerp = glm::vec3(-vDir.z, 0.0f, vDir.x);
	glBegin(GL_QUADS);

	const float fCoordMultiplierU = 0.05f;
	const float fCoordMultiplierV = 0.05f;
	float fZeroTexV = 0.0f;
	float fCurTexV = fHeight*fCoordMultiplierV;

	glm::vec3 vVert;
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; glTexCoord2f(0.0f, fCurTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; glTexCoord2f(fLength*fCoordMultiplierU, fCurTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vDir*fLength; glTexCoord2f(fLength*fCoordMultiplierU, fZeroTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner; glTexCoord2f(0.0f, fZeroTexV); glVertex3fv(glm::value_ptr(vVert));

	fZeroTexV = fCurTexV;
	fCurTexV += fWidth*fCoordMultiplierV;

	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; glTexCoord2f(0.0f, fCurTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; glTexCoord2f(fLength*fCoordMultiplierU, fCurTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; glTexCoord2f(fLength*fCoordMultiplierU, fZeroTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; glTexCoord2f(0.0f, fZeroTexV); glVertex3fv(glm::value_ptr(vVert));

	fZeroTexV = fCurTexV;
	fCurTexV += fHeight*fCoordMultiplierV;

	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; glTexCoord2f(0.0f, fCurTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; glTexCoord2f(fLength*fCoordMultiplierU, fCurTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vDir*fLength+vPerp*fWidth; glTexCoord2f(fLength*fCoordMultiplierU, fZeroTexV); glVertex3fv(glm::value_ptr(vVert));
	vVert = vCorner+vPerp*fWidth; glTexCoord2f(0.0f, fZeroTexV); glVertex3fv(glm::value_ptr(vVert));

	glEnd();
}

void renderFireDemoScene()
{
	glEnable(GL_TEXTURE_2D);

	sbSkyboxes[0].renderSkybox(fcMain.vEye);

	tWorldTextures[0].bindTexture();

	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 20.0f); glVertex3f(-200.0f, 0.0f, -200.0f);
	glTexCoord2f(20.0f, 20.0f); glVertex3f(200.0f, 0.0f, -200.0f);
	glTexCoord2f(20.0f, 0.0f); glVertex3f(200.0f, 0.0f, 200.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-200.0f, 0.0f, 200.0f);

	glEnd();

	renderWall(glm::vec3(-200.0f, 0.0f, 200.0f), glm::vec3(0.0f, 0.0f, -1.0f), 400.0f, 4.0f, 30.0f, 1);
	renderWall(glm::vec3(-200.0f, 0.0f, -200.0f), glm::vec3(1.0f, 0.0f, 0.0f), 400.0f, 4.0f, 30.0f, 1);
	renderWall(glm::vec3(200.0f, 0.0f, -200.0f), glm::vec3(0.0f, 0.0f, 1.0f), 400.0f, 4.0f, 30.0f, 1);
	renderWall(glm::vec3(200.0f, 0.0f, 200.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 400.0f, 4.0f, 30.0f, 1);

	psEngine->renderParticles();
	psEngine->updateParticles(appMain.sof(1.0f));
}