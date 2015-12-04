#pragma once

#define DLLEXPORT

#include "blaze_sys_common.h"

#include "blaze_particle.h"

#include "blaze_gen_pointGPU.h"
#include "blaze_gen_sphericalGPU.h"
#include "blaze_gen_boxGPU.h"
#include "blaze_gen_ellipsoidalGPU.h"

#include <glm/glm.hpp>
#include "shaders.h"

class BlazeTransformFeedbackParticleSystemEngine : public BlazeParticleSystemEngineCommon
{
public:
	bool initalizeParticleSystem(void* initData = NULL);

	void renderParticles();
	void updateParticles(float fTimePassed);

	UINT createPointParticleGenerator(float* a_fPosition, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);
	UINT createSphericalParticleGenerator(float* a_fPosition, float a_fRadius, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);
	UINT createBoxParticleGenerator(float* a_fCornerVertexA, float* a_fCornerVertexB, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);
	UINT createEllipsoidalParticleGenerator(float* a_fPosition, float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);

	bool deleteParticleGenerator(UINT uiGeneratorID);

	void clearAllParticles();
	void clearAllGenerators();
	bool releaseParticleSystem();

	int getNumParticles();
	int getNumGenerators();

private:
	UINT uiSampler;
	UINT uiTransFeedbackBuf;
	UINT uiQuery;

	CShader shVertexGen, shGeomGenPoint, shGeomGenSpherical, shGeomGenBox, shGeomGenEllipsoidal;
	CShader shVertexRender, shGeomRender, shFragRender;
	CShaderProgram spProgramGenPoint, spProgramGenSpherical, spProgramGenBox, spProgramGenEllipsoidal, spProgramRender;
};