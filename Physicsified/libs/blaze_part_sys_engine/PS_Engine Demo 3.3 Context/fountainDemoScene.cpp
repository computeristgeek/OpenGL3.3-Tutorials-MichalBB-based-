#include "common_header.h"

#include "win_OpenGLApp.h"
#include "texture.h"
#include "flyingCamera.h"
#include "freeTypeFont.h"
#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
			UINT uiID = psEngine->createSphericalParticleGenerator((float*)glm::value_ptr(glm::vec3(-50.0f+i*10.0f, 0.0f, -10.0f+j*20.0f)), 0.5f, (float*)glm::value_ptr(glm::vec3(-2.0f, 10.0f, -2.0f)), (float*)glm::value_ptr(glm::vec3(2.0f, 15.0f, 2.0f)), 0.02f, 0.02f, 40, 45, 2.0f, 2.5f, 0.15f);
			psEngine->setGeneratorGravityVector(uiID, (float*)glm::value_ptr(glm::vec3(0.0f, -10.0f, 0.0f)));
			psEngine->setGeneratorColor(uiID, 81, 120, 153);
		}
	}

	UINT uiID = psEngine->createSphericalParticleGenerator((float*)glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)), 2.0f, (float*)glm::value_ptr(glm::vec3(-5.0f, 28.0f, -5.0f)), (float*)glm::value_ptr(glm::vec3(5.0f, 35.0f, 5.0f)), 0.02f, 0.02f, 60, 75, 3.0f, 3.5f, 0.4f);
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

void renderFountainDemoScene(COpenGLControl* oglControl)
{
	spDirectionalLight.setUniform("projectionMatrix", oglControl->getProjectionMatrix());
	spDirectionalLight.setUniform("gSampler", 0);
	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));

	glm::mat4 mModelView = fcMain.look();
	glm::mat4 mModelToCamera;

	spDirectionalLight.setUniform("modelViewMatrix", glm::translate(mModelView, fcMain.vEye));
	sbSkyboxes[2].renderSkybox();

	glBindVertexArray(uiVAOs[2]);

	spDirectionalLight.setUniform("modelViewMatrix", &mModelView);

	// Render ground

	tWorldTextures[4].bindTexture();
	glDrawArrays(GL_TRIANGLES, 6, 6);

	tWorldTextures[5].bindTexture();
	glDrawArrays(GL_TRIANGLES, 12, 72);

	// Render water
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0);

	static float fAngle;
	fAngle += appMain.sof(1.3f);
	float fSine = float(sin(fAngle))*0.2f;

	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
	spDirectionalLight.setUniform("fAddToXTextureCoordinate", fSine);

	tWorldTextures[3].bindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisable(GL_BLEND);
	glDepthMask(1);

	psEngine->setProjectionMatrix(glm::value_ptr(*oglControl->getProjectionMatrix()));
	psEngine->setLookAt(fcMain.vEye.x, fcMain.vEye.y, fcMain.vEye.z, fcMain.vView.x, fcMain.vView.y, fcMain.vView.z, fcMain.vUp.x, fcMain.vUp.y, fcMain.vUp.z);
	float fTimePassed = appMain.sof(1.0f);
	psEngine->updateParticles(fTimePassed);
	psEngine->renderParticles();

}