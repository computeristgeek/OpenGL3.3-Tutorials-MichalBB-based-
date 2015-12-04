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

void generateWalls(CVertexBufferObject& stor, glm::vec3 vCorner, glm::vec3 vDir, float fLength, float fWidth, float fHeight)
{
	vDir = glm::normalize(vDir);

	glm::vec3 vPerp = glm::vec3(-vDir.z, 0.0f, vDir.x);

	const float fCoordMultiplierU = 0.05f;
	const float fCoordMultiplierV = 0.05f;
	float fZeroTexV = 0.0f;
	float fCurTexV = fHeight*fCoordMultiplierV;

	glm::vec3 vVert;
	glm::vec2 vCoord;
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; vCoord = glm::vec2(0.0f, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight;  vCoord = glm::vec2(fLength*fCoordMultiplierU, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength;  vCoord = glm::vec2(fLength*fCoordMultiplierU, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength;  vCoord = glm::vec2(fLength*fCoordMultiplierU, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner;  vCoord = glm::vec2(0.0f, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; vCoord = glm::vec2(0.0f, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);

	fZeroTexV = fCurTexV;
	fCurTexV += fWidth*fCoordMultiplierV;

	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; vCoord = glm::vec2(0.0f, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; vCoord = glm::vec2(fLength*fCoordMultiplierU, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; vCoord = glm::vec2(fLength*fCoordMultiplierU, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; vCoord = glm::vec2(fLength*fCoordMultiplierU, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight; vCoord = glm::vec2(0.0f, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; vCoord = glm::vec2(0.0f, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);

	fZeroTexV = fCurTexV;
	fCurTexV += fHeight*fCoordMultiplierV;

	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; vCoord = glm::vec2(0.0f, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; vCoord = glm::vec2(fLength*fCoordMultiplierU, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+vPerp*fWidth; vCoord = glm::vec2(fLength*fCoordMultiplierU, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vDir*fLength+vPerp*fWidth; vCoord = glm::vec2(fLength*fCoordMultiplierU, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+vPerp*fWidth; vCoord = glm::vec2(0.0f, fZeroTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
	vVert = vCorner+glm::vec3(0.0f, 1.0f, 0.0f)*fHeight+vPerp*fWidth; vCoord = glm::vec2(0.0f, fCurTexV);
	stor.addData(&vVert, sizeof(float)*3); stor.addData(&vCoord, sizeof(float)*2);
}

void renderFireDemoScene(COpenGLControl* oglControl)
{
	spDirectionalLight.setUniform("projectionMatrix", oglControl->getProjectionMatrix());
	spDirectionalLight.setUniform("gSampler", 0);
	spDirectionalLight.setUniform("vColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));

	glm::mat4 mModelView = fcMain.look();
	glm::mat4 mModelToCamera;

	spDirectionalLight.setUniform("modelViewMatrix", glm::translate(mModelView, fcMain.vEye));
	sbSkyboxes[0].renderSkybox();

	glBindVertexArray(uiVAOs[0]);

	spDirectionalLight.setUniform("modelViewMatrix", &mModelView);

	// Render ground

	tWorldTextures[0].bindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	tWorldTextures[1].bindTexture();
	glDrawArrays(GL_TRIANGLES, 6, 72);

	psEngine->setProjectionMatrix(glm::value_ptr(*oglControl->getProjectionMatrix()));
	psEngine->setLookAt(fcMain.vEye.x, fcMain.vEye.y, fcMain.vEye.z, fcMain.vView.x, fcMain.vView.y, fcMain.vView.z, fcMain.vUp.x, fcMain.vUp.y, fcMain.vUp.z);
	float fTimePassed = appMain.sof(1.0f);
	psEngine->updateParticles(fTimePassed);
	psEngine->renderParticles();

	if(Keys::onekey('H'))BlazeEffects::deleteEffect(4);
}