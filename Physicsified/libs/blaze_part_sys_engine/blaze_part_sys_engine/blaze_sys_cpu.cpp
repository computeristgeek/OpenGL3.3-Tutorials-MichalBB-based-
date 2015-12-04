#include "common_header.h"

#include "blaze_sys_cpu.h"

bool BlazeParticleSystemEngineCPU::initalizeParticleSystem(void* initData)
{
	if(bInitialized)return false;

	iFirstFreeIndex = 0;
	gGenerators.reserve(400);

	bInitialized = true;

	return true;
}

void BlazeParticleSystemEngineCPU::updateParticles(float fTimePassed)
{
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)
	{
		if(gGenerators[i]->isDead())
		{
			deleteParticleGenerator(i);
			continue;
		}
		BlazeParticleGeneratorCPU* gGen =  dynamic_cast<BlazeParticleGeneratorCPU*>(gGenerators[i]);
		gGen->generateParticles(fTimePassed, &pParticles);
	}
	FOR(i, ESZ(pParticles))
	{
		FOR(j, 3)
		{
			pParticles[i].fPosition[j] += pParticles[i].fVelocity[j]*fTimePassed;
			pParticles[i].fVelocity[j] += pParticles[i].fGravityVector[j]*fTimePassed;
		}
		pParticles[i].fLifeTimeSpan -= fTimePassed;
		if(pParticles[i].fLifeTimeSpan < 0.0f)
		{
			int lastIndex = pParticles.size()-1;
			pParticles[i] = pParticles[lastIndex];
			pParticles.pop_back();
			i--;
		}
	}
}

bool BlazeParticleSystemEngineCPU::deleteParticleGenerator(UINT uiGeneratorID)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	if(uiGeneratorID >= ESZ(gGenerators))
		return false;

	delete gGenerators[uiGeneratorID];
	gGenerators[uiGeneratorID] = NULL;

	return true;
}

UINT BlazeParticleSystemEngineCPU::createPointParticleGenerator(float* a_fPosition, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazePointParticleGeneratorCPU* gNew = new BlazePointParticleGeneratorCPU;

	gNew->BlazePointParticleGenerator::initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->setPosition(a_fPosition);

	return addGenerator(gNew);
}

UINT BlazeParticleSystemEngineCPU::createSphericalParticleGenerator(float* a_fPosition, float a_fRadius, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazeSphericalParticleGeneratorCPU* gNew = new BlazeSphericalParticleGeneratorCPU;

	gNew->BlazeSphericalParticleGenerator::initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->setPosition(a_fPosition);
	gNew->setSphereRadius(a_fRadius);

	return addGenerator(gNew);
}

UINT BlazeParticleSystemEngineCPU::createBoxParticleGenerator(float* a_fCornerVertexA, float* a_fCornerVertexB, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazeBoxParticleGeneratorCPU* gNew = new BlazeBoxParticleGeneratorCPU;

	gNew->BlazeBoxParticleGenerator::initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->setBoxOppositeCorners(a_fCornerVertexA, a_fCornerVertexB);

	return addGenerator(gNew);
}

UINT BlazeParticleSystemEngineCPU::createEllipsoidalParticleGenerator(float* a_fPosition, float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazeEllipsoidalParticleGeneratorCPU* gNew = new BlazeEllipsoidalParticleGeneratorCPU;

	gNew->BlazeEllipsoidalParticleGenerator::initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->setPosition(a_fPosition);
	gNew->setEllipseRadii(a_fRadiusX, a_fRadiusY, a_fRadiusZ);

	return addGenerator(gNew);
}

int BlazeParticleSystemEngineCPU::getNumParticles()
{
	return ESZ(pParticles);
}

int BlazeParticleSystemEngineCPU::getNumGenerators()
{
	int iRes = 0;
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)iRes++;

	return iRes;
}

bool BlazeParticleSystemEngineCPU::releaseParticleSystem()
{
	if(!bInitialized)return false;
	clearAllGenerators();
	bInitialized = false;

	return true;
}

void BlazeParticleSystemEngineCPU::clearAllParticles()
{
	pParticles.clear();
}

void BlazeParticleSystemEngineCPU::clearAllGenerators()
{
	FOR(i, ESZ(gGenerators))deleteParticleGenerator(i);
}