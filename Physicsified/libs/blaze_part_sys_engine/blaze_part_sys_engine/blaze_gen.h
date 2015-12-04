#pragma once

#define BLAZE_PART_GEN_TYPE_POINT		0x00
#define BLAZE_PART_GEN_TYPE_SPHERE		0x01
#define BLAZE_PART_GEN_TYPE_BOX			0x02
#define BLAZE_PART_GEN_TYPE_ELLIPSE		0x03

#include "blaze_particle.h"

class BlazeParticleGenerator
{
public:
	void initGenerator(float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns = -1);

	void regenerateNextTime();
	void setGeneratorColor(BYTE a_r, BYTE a_g, BYTE a_b);
	void setGravityVector(float* a_fGravity);
	void setSpawnRate(float fNewSpawnEveryMin, float fNewSpawnEveryMax, int iNewMinCount, int iNewMaxCount);
	void setParticleSize(float a_fNewParticleSize);
	bool isDead();

	int getGeneratorType();

	virtual ~BlazeParticleGenerator() = 0;

protected:
	int iGenType;
	float fVelocityRangeMin[3];
	float fVelocityRangeDif[3];
	float fSpawnEveryMin;
	float fSpawnEveryDif;
	int countMin;
	int countDif;
	float fLifeMin;
	float fLifeDif;
	float fParticleSizes;

	int maxSpawns;
	int numSpawns;

	float fNextTime;
	float fPassedTime;

	int iNewCount;

	float fGravityVector[3];

	float fColor[3];
};

float grandf(float fMin, float fAdd);