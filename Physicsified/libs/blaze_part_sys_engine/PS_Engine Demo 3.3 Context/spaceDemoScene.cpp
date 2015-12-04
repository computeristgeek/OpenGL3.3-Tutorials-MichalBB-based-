#include "common_header.h"

#include "win_OpenGLApp.h"
#include "texture.h"
#include "flyingCamera.h"
#include "freeTypeFont.h"
#include "skybox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/blaze_part_sys_engine.h"
#include "../include/blaze_effects.h"

#include "demoScenes.h"

class CFlyingCube
{
public:
	glm::vec3 vPos, vDir;
	float fTimeLeft;
	float fRot;
	UINT uiGenID;

	CFlyingCube()
	{

	}
	CFlyingCube(glm::vec3 a_vPos, glm::vec3 a_vDir, float a_fTimeLeft, UINT a_uiGenID)
	{
		vPos = a_vPos;
		vDir = a_vDir;
		fTimeLeft = a_fTimeLeft;
		fRot = 0.0f;
		uiGenID = a_uiGenID;
	}
};

vector<CFlyingCube> flyingCubes;

void prepareSpaceDemoScene()
{
	flyingCubes.clear();

	float fVelocity[] = {0.0f, 0.0f, -15.0f};

	psEngine->setParticleTexture(tParticleTexture.getTextureID());

	int each = 17;
	float fStep = 80.0f/(each-1);

	glm::vec3 vPos = glm::vec3(-40.0f, 0.0f, 0.0f);

	FOR(x, each)
	{
		UINT uiGenerator = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocity, fVelocity, 0.06f, 0.06f, 1, 1, 8.5f, 8.7f, 1.3f);
		psEngine->setGeneratorColor(uiGenerator, 51, 189, 255);
		vPos.x += fStep;
	}

	vPos.y += fVelocity[1]*8.6f;
	vPos.z += fVelocity[2]*8.6f;
	vPos.x = -40.0f;

	glm::vec3 vVector = glm::vec3(0.0f, 4.0f, -15.0f);
	vVector = glm::normalize(vVector);
	vVector *= 15.0f;

	memcpy(fVelocity, glm::value_ptr(vVector), sizeof(float)*3);

	FOR(x, each)
	{
		UINT uiGenerator = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocity, fVelocity, 0.06f, 0.06f, 1, 1, 5.5f, 5.7f, 1.3f);
		psEngine->setGeneratorColor(uiGenerator, 51, 189, 255);
		vPos.x += fStep;
	}

	vPos.y += fVelocity[1]*5.6f;
	vPos.z += fVelocity[2]*5.6f;
	vPos.x = -40.0f;

	fVelocity[1] *= -1.0f;
	FOR(x, each)
	{
		UINT uiGenerator = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocity, fVelocity, 0.06f, 0.06f, 1, 1, 5.5f, 5.7f, 1.3f);
		psEngine->setGeneratorColor(uiGenerator, 51, 189, 255);
		vPos.x += fStep;
	}

	vPos.y += fVelocity[1]*5.6f;
	vPos.z += fVelocity[2]*5.6f;
	vPos.x = -40.0f;

	fVelocity[1] = 0.0f; fVelocity[2] = -15.0f;

	FOR(x, each)
	{
		UINT uiGenerator = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocity, fVelocity, 0.06f, 0.06f, 1, 1, 8.5f, 8.7f, 1.3f);
		psEngine->setGeneratorColor(uiGenerator, 51, 189, 255);
		vPos.x += fStep;
	}
}

void renderCube(glm::vec3 vPos, float fSize, float fRotX, float fRotY, float fRotZ)
{
	glm::mat4 mModelMatrix = glm::translate(glm::mat4(1.0), vPos);
	mModelMatrix = glm::rotate(mModelMatrix,fRotX, glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix,fRotY, glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, fRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(fSize, fSize, fSize));

	spColor.setUniform("matrices.modelMatrix", mModelMatrix);

	glm::vec3 vColors[] = 
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f)
	};
	FOR(i, 6)
	{
		spColor.setUniform("vColor", glm::vec4(vColors[i].x, vColors[i].y, vColors[i].z, 1.0f));
		glDrawArrays(GL_TRIANGLES, 36+i*6, 6);
	}
}

void renderSpaceDemoScene(COpenGLControl* oglControl)
{
	spDirectionalLight.setUniform("projectionMatrix", oglControl->getProjectionMatrix());
	spDirectionalLight.setUniform("gSampler", 0);
	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));

	glm::mat4 mModelView = fcMain.look();
	glm::mat4 mModelToCamera;

	spDirectionalLight.setUniform("modelViewMatrix", glm::translate(mModelView, fcMain.vEye));
	sbSkyboxes[1].renderSkybox();

	glBindVertexArray(uiVAOs[1]);

	spDirectionalLight.setUniform("modelViewMatrix", &mModelView);

	// Render Walls

	tWorldTextures[2].bindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	spColor.useProgram();
	spColor.setUniform("matrices.projectionMatrix", oglControl->getProjectionMatrix());
	spColor.setUniform("matrices.viewMatrix", fcMain.look());

	FOR(i, ESZ(flyingCubes))
	{
		psEngine->setPointGeneratorPosition(flyingCubes[i].uiGenID, glm::value_ptr(flyingCubes[i].vPos));
		renderCube(flyingCubes[i].vPos, 2.0f, flyingCubes[i].fRot, flyingCubes[i].fRot, flyingCubes[i].fRot);
		flyingCubes[i].fRot += appMain.sof(180.0f);
		flyingCubes[i].fTimeLeft -= appMain.sof(1.0f);
		flyingCubes[i].vPos += flyingCubes[i].vDir*appMain.sof(22.0f);

		if(flyingCubes[i].fTimeLeft < 0.0f)
		{
			float fVelocityMin[] = {-12.0f, -12.0f, -12.0f};
			float fVelocityMax[] = {12.0f, 12.0f, 12.0f};
			UINT uiGen = psEngine->createSphericalParticleGenerator(glm::value_ptr(flyingCubes[i].vPos), 2.0f, fVelocityMin, fVelocityMax, 0.0f, 0.0f, 500, 650, 2.0f, 3.0f, 0.5f, 1);
			psEngine->deleteParticleGenerator(flyingCubes[i].uiGenID);
			psEngine->setGeneratorColor(uiGen, 255, 128, 0);
			flyingCubes[i] = flyingCubes[ESZ(flyingCubes)-1];

			flyingCubes.pop_back();
			i--;
		}
	}

	static float fRapid = 0.0f;
	if(Keys::key(VK_LBUTTON))
	{
		fRapid += appMain.sof(1.0f);
		if(fRapid > 0.05f)
		{
			fRapid = 0.0f;
			glm::vec3 vNewPos = fcMain.vEye+(fcMain.vView-fcMain.vEye)*4.0f;
			glm::vec3 vNewDir = fcMain.vView-fcMain.vEye;
			glm::vec3 vVel = vNewDir*(-17.0f);
			UINT uiID = psEngine->createPointParticleGenerator(glm::value_ptr(vNewPos), glm::value_ptr(vVel), glm::value_ptr(vVel), 0.02f, 0.02f, 1, 1, 0.6f, 1.0f, 2.4f);
			psEngine->setGeneratorColor(uiID, 232, 232, 232);
			flyingCubes.push_back(CFlyingCube(vNewPos, vNewDir, 3.0f, uiID));
		}
	}
	else fRapid = 0.06f;

	psEngine->setProjectionMatrix(glm::value_ptr(*oglControl->getProjectionMatrix()));
	psEngine->setLookAt(fcMain.vEye.x, fcMain.vEye.y, fcMain.vEye.z, fcMain.vView.x, fcMain.vView.y, fcMain.vView.z, fcMain.vUp.x, fcMain.vUp.y, fcMain.vUp.z);
	float fTimePassed = appMain.sof(1.0f);
	psEngine->updateParticles(fTimePassed);
	psEngine->renderParticles();

}