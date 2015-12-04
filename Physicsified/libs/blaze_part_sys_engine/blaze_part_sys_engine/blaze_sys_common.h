#pragma once

#define DLLEXPORT

#include "../include/blaze_part_sys_engine.h"

#include "blaze_gen.h"
#include "blaze_gen_point.h"
#include "blaze_gen_spherical.h"
#include "blaze_gen_box.h"
#include "blaze_gen_ellipsoidal.h"

class BlazeParticleSystemEngineCommon : public BlazeParticleSystemEngine
{
public:
	void setParticleTexture(unsigned int a_uiTexture);
	bool setGeneratorColor(UINT uiGeneratorID, BYTE r, BYTE g, BYTE b);
	bool setGeneratorGravityVector(UINT uiGeneratorID, float* fGravityVector);
	bool setGeneratorSpawnRate(UINT uiGeneratorID, float fNewSpawnEveryMin, float fNewSpawnEveryMax, int iNewMinCount, int iNewMaxCount);
	bool setGeneratorParticleSize(UINT uiGeneratorID, float fNewParticleSize);

	bool setPointGeneratorPosition(UINT uiGeneratorID, float* fNewPositon);
	bool setSphericalGeneratorCenter(UINT uiGeneratorID, float* fNewPosition);
	bool setSphericalGeneratorRadius(UINT uiGeneratorID, float fNewRadius);
	bool setBoxGeneratorCorners(UINT uiGeneratorID, float* fNewCornerVertexA, float* fNewCornerVertexB);
	bool setEllipsoidalGeneratorCenter(UINT uiGeneratorID, float* fNewPosition);
	bool setEllipsoidalGeneratorRadii(UINT uiGeneratorID, float fNewRadiusX, float fNewRadiusY, float fNewRadiusZ);

	void setProjectionMatrix(float* a_mProj);
	void setLookAt(float fEyeX, float fEyeY, float fEyeZ, float fViewX, float fViewY, float fViewZ, float fUpX, float fUpY, float fUpZ);

	BlazeParticleSystemEngineCommon();

protected:
	bool bInitialized;

	UINT addGenerator(BlazeParticleGenerator* gGen);

	vector<BlazeParticleGenerator*> gGenerators;
	int iFirstFreeIndex;

	UINT uiTexture;

	glm::vec3 vViewVector, vQuad1, vQuad2;
	glm::mat4 mProj, mView;
};