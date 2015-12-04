#pragma once

#include "blaze_sys_common.h"

#include "blaze_gen_pointCPU.h"
#include "blaze_gen_sphericalCPU.h"
#include "blaze_gen_boxCPU.h"
#include "blaze_gen_ellipsoidalCPU.h"

class BlazeParticleSystemEngineCPU : public BlazeParticleSystemEngineCommon
{
public:
	bool initalizeParticleSystem(void* initData = NULL);

	void updateParticles(float fTimePassed);

	UINT createPointParticleGenerator(float* a_fPosition, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);
	UINT createSphericalParticleGenerator(float* a_fPosition, float a_fRadius, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);
	UINT createBoxParticleGenerator(float* a_fCornerVertexA, float* a_fCornerVertexB, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);
	UINT createEllipsoidalParticleGenerator(float* a_fPosition, float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);

	void clearAllParticles();
	void clearAllGenerators();
	bool releaseParticleSystem();

	int getNumParticles();
	int getNumGenerators();

	bool deleteParticleGenerator(UINT uiGeneratorID);

protected:

	vector<BlazeParticle> pParticles;
};