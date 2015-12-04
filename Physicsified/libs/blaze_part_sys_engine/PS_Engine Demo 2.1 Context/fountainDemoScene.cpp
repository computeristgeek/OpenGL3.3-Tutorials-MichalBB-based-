#include "common_header.h"

#include "win_OpenGLApp.h"
#include "texture.h"
#include "flyingCamera.h"
#include "freeTypeFont.h"
#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>

#include "demoScenes.h"

float grandf(float fMin, float fAdd)
{
	float fRandom = float(rand()%(RAND_MAX+1))/float(RAND_MAX);
	return fMin+fAdd*fRandom;
}

void prepareFountainDemoScene()
{
	psEngine->setParticleTexture(tParticleTexture.getTextureID());
	FOR(i, 10)
	{
		FOR(j, 2)
		{
			UINT uiID = psEngine->createSphericalParticleGenerator((float*)glm::value_ptr(glm::vec3(-50.0f+i*10.0f, 0.0f, -10.0f+j*20.0f)), 0.5f, (float*)glm::value_ptr(glm::vec3(-2.0f, 10.0f, -2.0f)), (float*)glm::value_ptr(glm::vec3(2.0f, 15.0f, 2.0f)), 0.02f, 0.02f, 20, 25, 2.0f, 2.5f, 0.15f);
			psEngine->setGeneratorGravityVector(uiID, (float*)glm::value_ptr(glm::vec3(0.0f, -10.0f, 0.0f)));
			psEngine->setGeneratorColor(uiID, 81, 120, 153);
		}
	}

	UINT uiID = psEngine->createSphericalParticleGenerator((float*)glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)), 2.0f, (float*)glm::value_ptr(glm::vec3(-5.0f, 28.0f, -5.0f)), (float*)glm::value_ptr(glm::vec3(5.0f, 35.0f, 5.0f)), 0.02f, 0.02f, 30, 40, 3.0f, 3.5f, 0.4f);
	psEngine->setGeneratorGravityVector(uiID, (float*)glm::value_ptr(glm::vec3(0.0f, -15.0f, 0.0f)));
	psEngine->setGeneratorColor(uiID, 81, 120, 153);

	int steps = 15;
	float fEach = 3.1415f*2.0f/float(steps-1);
	FOR(i, steps)
	{
		float fSine = sin(fEach*i);
		float fCosine = cos(fEach*i);
		glm::vec3 vDir = glm::vec3(fSine, 0.0f, fCosine)*12.0f;
		glm::vec3 vDir2 = vDir+glm::vec3(grandf(-0.02f, 0.02f), grandf(-0.02f, 0.02f), grandf(-0.02f, 0.02f));
		vDir.y += 25.0f;
		UINT uiID = psEngine->createPointParticleGenerator((float*)glm::value_ptr(glm::vec3(fSine*2.5f, 0.0f, fCosine*2.5f)), (float*)glm::value_ptr(vDir), (float*)glm::value_ptr(vDir), 0.01f, 0.01f, 1, 2, 4.0f, 4.5f, 0.6f);
		psEngine->setGeneratorGravityVector(uiID, (float*)glm::value_ptr(glm::vec3(0.0f, -15.0f, 0.0f)));
		psEngine->setGeneratorColor(uiID, 81, 120, 153);
	}
}

void renderFountainDemoScene()
{
	glEnable(GL_TEXTURE_2D);

	sbSkyboxes[2].renderSkybox(fcMain.vEye);

	tWorldTextures[4].bindTexture();

	glBegin(GL_QUADS);
	glColor4ub(255, 255, 255, 150);
	glTexCoord2f(0.0f, 20.0f); glVertex3f(-100.0f, -10.0f, -100.0f);
	glTexCoord2f(20.0f, 20.0f); glVertex3f(100.0f, -10.0f, -100.0f);
	glTexCoord2f(20.0f, 0.0f); glVertex3f(100.0f, -10.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -10.0f, 100.0f);
	glEnd();

	renderWall(glm::vec3(-100.0f, -10.0f, 100.0f), glm::vec3(0.0f, 0.0f, -1.0f), 200.0f, 4.0f, 30.0f, 5);
	renderWall(glm::vec3(-100.0f, -10.0f, -100.0f), glm::vec3(1.0f, 0.0f, 0.0f), 200.0f, 4.0f, 30.0f, 5);
	renderWall(glm::vec3(100.0f, -10.0f, -100.0f), glm::vec3(0.0f, 0.0f, 1.0f), 200.0f, 4.0f, 30.0f, 5);
	renderWall(glm::vec3(100.0f, -10.0f, 100.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 200.0f, 4.0f, 30.0f, 5);

	tWorldTextures[3].bindTexture();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0);

	static float fAngle;
	fAngle += appMain.sof(1.3f);
	float fSine = float(sin(fAngle))*0.2f;

	glBegin(GL_QUADS);
	glColor4ub(255, 255, 255, 150);
	glTexCoord2f(0.0f+fSine, 20.0f); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(20.0f+fSine, 20.0f); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2f(20.0f+fSine, 0.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f+fSine, 0.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(1);

	psEngine->renderParticles();
	psEngine->updateParticles(appMain.sof(1.0f));
}