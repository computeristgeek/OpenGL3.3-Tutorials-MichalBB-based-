#pragma once

#ifndef _AFXDLL
#define _AFXDLL
#endif

#if defined(DLLEXPORT) // inside DLL
#define BLAZE_PART_SYS_ENGINE_API   __declspec(dllexport)
#else // outside DLL
#define BLAZE_PART_SYS_ENGINE_API   __declspec(dllimport)
#endif

#ifndef UINT
#define UINT unsigned int
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

class BlazeParticleSystemEngine
{
public:
	virtual bool initalizeParticleSystem(void* initData = NULL) = 0;

	virtual void renderParticles() = 0;
	virtual void updateParticles(float fTimePassed) = 0;
	virtual void setParticleTexture(unsigned int a_uiTexture) = 0;

	virtual UINT createPointParticleGenerator(float* a_fPosition, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_numSpawns = -1) = 0;
	virtual UINT createSphericalParticleGenerator(float* a_fPosition, float a_fRadius, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1) = 0;
	virtual UINT createBoxParticleGenerator(float* a_fCornerVertexA, float* a_fCornerVertexB, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1) = 0;
	virtual UINT createEllipsoidalParticleGenerator(float* a_fPosition, float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1) = 0;

	virtual bool setGeneratorColor(UINT uiGeneratorID, BYTE r, BYTE g, BYTE b) = 0;
	virtual bool setGeneratorGravityVector(UINT uiGeneratorID, float* fGravityVector) = 0;
	virtual bool setGeneratorSpawnRate(UINT uiGeneratorID, float fNewSpawnEveryMin, float fNewSpawnEveryMax, int iNewMinCount, int iNewMaxCount) = 0;
	virtual bool setGeneratorParticleSize(UINT uiGeneratorID, float fNewParticleSize) = 0;

	virtual bool setPointGeneratorPosition(UINT uiGeneratorID, float* fNewPosition) = 0;
	virtual bool setSphericalGeneratorCenter(UINT uiGeneratorID, float* fNewPosition) = 0;
	virtual bool setSphericalGeneratorRadius(UINT uiGeneratorID, float fNewRadius) = 0;
	virtual bool setBoxGeneratorCorners(UINT uiGeneratorID, float* fNewCornerVertexA, float* fNewCornerVertexB) = 0;
	virtual bool setEllipsoidalGeneratorCenter(UINT uiGeneratorID, float* fNewPosition) = 0;
	virtual bool setEllipsoidalGeneratorRadii(UINT uiGeneratorID, float fNewRadiusX, float fNewRadiusY, float fNewRadiusZ) = 0;

	virtual int getNumParticles() = 0;
	virtual int getNumGenerators() = 0;

	virtual void setProjectionMatrix(float* a_mProj) = 0;
	virtual void setLookAt(float fEyeX, float fEyeY, float fEyeZ, float fViewX, float fViewY, float fViewZ, float fUpX, float fUpY, float fUpZ) = 0;

	virtual bool deleteParticleGenerator(UINT uiGeneratorID) = 0;
	virtual void clearAllParticles() = 0;
	virtual void clearAllGenerators() = 0;

	virtual bool releaseParticleSystem() = 0;
};

BLAZE_PART_SYS_ENGINE_API BlazeParticleSystemEngine* createSimpleParticleSystem();
BLAZE_PART_SYS_ENGINE_API BlazeParticleSystemEngine* createPointSpritesParticleSystem();
BLAZE_PART_SYS_ENGINE_API BlazeParticleSystemEngine* createTransformFeedbackParticleSystem();
