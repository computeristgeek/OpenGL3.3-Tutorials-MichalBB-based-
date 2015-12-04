#include "common_header.h"

#include "blaze_gen.h"

// Pure virtual destructor body
BlazeParticleGenerator::~BlazeParticleGenerator()
{

}

void BlazeParticleGenerator::initGenerator(float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	memcpy(fVelocityRangeMin, a_fVelocityRangeMin, sizeof(float)*3);
	memcpy(fVelocityRangeDif, a_fVelocityRangeMax, sizeof(float)*3);
	FOR(i, 3)fVelocityRangeDif[i] -= fVelocityRangeMin[i];
	fSpawnEveryMin = a_fSpawnEveryMin;
	fSpawnEveryDif = a_fSpawnEveryMax-a_fSpawnEveryMin;
	countMin = a_countMin;
	countDif = a_countMax-a_countMin;
	fLifeMin = a_fLifeMin;
	fLifeDif = a_fLifeMax-a_fLifeMin;
	maxSpawns =  a_maxSpawns;

	fParticleSizes = a_fParticleSizes;

	fColor[0] = 1.0f;
	fColor[1] = 0.5f;
	fColor[2] = 0.0f;

	fGravityVector[0] = fGravityVector[1] = fGravityVector[2] = 0.0f;

	regenerateNextTime();

	numSpawns = 0;
}

float grandf(float fMin, float fAdd)
{
	float fRandom = float(rand()%(RAND_MAX+1))/float(RAND_MAX);
	return fMin+fAdd*fRandom;
}

void BlazeParticleGenerator::regenerateNextTime()
{
	fNextTime = grandf(fSpawnEveryMin, fSpawnEveryDif);
	fPassedTime = 0.0f;
	numSpawns++;
	iNewCount = int(grandf(float(countMin), float(countDif)));
}

void BlazeParticleGenerator::setGeneratorColor(BYTE a_r, BYTE a_g, BYTE a_b)
{
	fColor[0] = float(a_r)/255.0f;
	fColor[1] = float(a_g)/255.0f;
	fColor[2] = float(a_b)/255.0f;
}

void BlazeParticleGenerator::setGravityVector(float* a_fGravity)
{
	memcpy(fGravityVector, a_fGravity, sizeof(float)*3);
}

void BlazeParticleGenerator::setSpawnRate(float fNewSpawnEveryMin, float fNewSpawnEveryMax, int iNewMinCount, int iNewMaxCount)
{
	fSpawnEveryMin = fNewSpawnEveryMin;
	fSpawnEveryDif = fNewSpawnEveryMax-fNewSpawnEveryMin;
	
	countMin = iNewMinCount;
	countDif = iNewMaxCount-iNewMinCount;
}

void BlazeParticleGenerator::setParticleSize(float a_fNewParticleSize)
{
	fParticleSizes = a_fNewParticleSize;
}

bool BlazeParticleGenerator::isDead()
{
	return maxSpawns != -1 && numSpawns >= maxSpawns;
}

int BlazeParticleGenerator::getGeneratorType()
{
	return iGenType;
}