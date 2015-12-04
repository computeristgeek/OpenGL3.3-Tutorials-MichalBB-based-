#define DLLEXPORT

#include "common_header.h"
#include "../include/blaze_effects.h"

BlazeParticleSystemEngine* psEngine = NULL;
vector< vector<UINT> > listOfEffects;

#pragma comment(lib, "../lib/blaze_part_sys_engine_d.lib")

UINT getFreeIndex()
{
	UINT uiRes = ESZ(listOfEffects);
	FOR(i, ESZ(listOfEffects))
		if(ESZ(listOfEffects[i]) == 0)return i;
	listOfEffects.push_back(vector<UINT>(0));
	return uiRes;
}

BLAZE_EFFECTS_API void BlazeEffects::setParticleSystemEngine(BlazeParticleSystemEngine* a_psEngine)
{
	psEngine = a_psEngine;
}

BLAZE_EFFECTS_API UINT BlazeEffects::createFireRing(float* fCenter, float fRadius, int numSources, BYTE r, BYTE g, BYTE b)
{
	if(psEngine == NULL)	
		return 0;

	glm::vec3 vCenter(fCenter[0], fCenter[1], fCenter[2]);

	float fVelocityMin[] = {-0.1f, 1.0f, 0.1f};
	float fVelocityMax[] = {0.1f, 3.0f, 0.1f};

	float fAngle = 0.0f;
	int iNum = numSources;

	UINT uiIndex = getFreeIndex();

	FOR(i, iNum)
	{
		float fSine = float(sin(fAngle*3.14145f/180.0f));
		float fCosine = float(cos(fAngle*3.14145f/180.0f));
		glm::vec3 vPos = vCenter+glm::vec3(fCosine, 0.0f, fSine)*fRadius;
		UINT uiID = psEngine->createSphericalParticleGenerator(glm::value_ptr(vPos), 3.0f, fVelocityMin, fVelocityMax, 0.05f, 0.2f, 20, 25, 2.0f, 4.5f, 0.5f);
		psEngine->setGeneratorColor(uiID, r, g, b);
		fAngle += 360.0f/float(iNum-1);

		listOfEffects[uiIndex].push_back(uiID);
	}

	return uiIndex;
}

BLAZE_EFFECTS_API UINT BlazeEffects::createFireGrid(glm::vec3 fGridCenter, float fGridSize, int iSubDivisions, BYTE r, BYTE g, BYTE b)
{
	if(psEngine == NULL)	
		return 0;

	glm::vec3 vGridCenter(fGridCenter[0], fGridCenter[1], fGridCenter[2]);
	float fStep = fGridSize/(iSubDivisions-1);

	float fVelocityMinX[] = {-15.0f, 0.0f, 0.0f};
	float fVelocityMaxX[] = {-15.0f, 0.0f, 0.0f};

	UINT uiIndex = getFreeIndex();

	FOR(y, iSubDivisions)
	{
		FOR(x, iSubDivisions)
		{
			glm::vec3 vPos = glm::vec3(vGridCenter.x+fGridSize/2, vGridCenter.y-fGridSize/2+fStep*float(y), vGridCenter.z-fGridSize/2+float(x)*fStep);
			UINT uiID = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocityMinX, fVelocityMaxX, 0.03f, 0.03f, 1, 1, 3.5f, 3.7f, 0.6f);
			psEngine->setGeneratorColor(uiID, r, g, b);
			listOfEffects[uiIndex].push_back(uiID);
		}
	}

	float fVelocityMinY[] = {0.0f, 15.0f, 0.0f};
	float fVelocityMaxY[] = {0.0f, 15.0f, 0.0f};

	FOR(z, iSubDivisions)
	{
		FOR(x, iSubDivisions)
		{
			glm::vec3 vPos = glm::vec3(vGridCenter.x-fGridSize/2+fStep*float(x), vGridCenter.y-fGridSize/2, vGridCenter.z-fGridSize/2+float(z)*fStep);
			UINT uiID = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocityMinY, fVelocityMaxY, 0.03f, 0.03f, 1, 1, 3.5f, 3.7f, 0.6f);
			listOfEffects[uiIndex].push_back(uiID);
		}
	}

	float fVelocityMinZ[] = {0.0f, 0.0f, -15.0f};
	float fVelocityMaxZ[] = {0.0f, 0.0f, -15.0f};

	FOR(y, iSubDivisions)
	{
		FOR(x, iSubDivisions)
		{
			glm::vec3 vPos = glm::vec3(vGridCenter.x-fGridSize/2+fStep*float(x), vGridCenter.y-fGridSize/2+float(y)*fStep, vGridCenter.z+fGridSize/2);
			UINT uiID = psEngine->createPointParticleGenerator(glm::value_ptr(vPos), fVelocityMinZ, fVelocityMaxZ, 0.03f, 0.03f, 1, 1, 3.5f, 3.7f, 0.6f);
			listOfEffects[uiIndex].push_back(uiID);
		}
	}

	return uiIndex;
}

BLAZE_EFFECTS_API UINT BlazeEffects::createFire(float* fFireCenter, float* fFireSize, BYTE r, BYTE g, BYTE b)
{
	if(psEngine == NULL)
		return 0;

	float fCornasA[] = {fFireCenter[0]-fFireSize[0], fFireCenter[1]-fFireSize[1], fFireCenter[2]-fFireSize[2]};
	float fCornasB[] = {fFireCenter[0]+fFireSize[0], fFireCenter[1]+fFireSize[1], fFireCenter[2]+fFireSize[2]};

	float fVelocityMin[] = {-0.8f, 1.6f, -0.8f};
	float fVelocityMax[] = {0.8f, 3.6f, 0.8f};

	UINT uiIndex = getFreeIndex();

	UINT uiID = psEngine->createBoxParticleGenerator(fCornasA, fCornasB, fVelocityMin, fVelocityMax, 0.02f, 0.1f, 20, 35, 2.0f, 4.5f, 0.6f);
	psEngine->setGeneratorColor(uiID, r, g, b);
	listOfEffects[uiIndex].push_back(uiID);

	glm::vec3 vPos = glm::vec3(fFireCenter[0], fFireCenter[1], fFireCenter[2]);
	uiID = psEngine->createSphericalParticleGenerator(glm::value_ptr(vPos), 5.0f, fVelocityMin, fVelocityMax, 0.05f, 0.2f, 10, 20, 2.0f, 4.5f, 0.5f);
	psEngine->setGeneratorColor(uiID, r, g, b);
	listOfEffects[uiIndex].push_back(uiID);

	return uiIndex;
}

BLAZE_EFFECTS_API bool BlazeEffects::deleteEffect(UINT uiEffectID)
{
	if(psEngine == NULL)	
		return false;

	if(uiEffectID >= ESZ(listOfEffects))
		return false;

	FOR(i, ESZ(listOfEffects[uiEffectID]))
		psEngine->deleteParticleGenerator(listOfEffects[uiEffectID][i]);
	listOfEffects[uiEffectID].clear();

	return true;
}

BLAZE_EFFECTS_API bool BlazeEffects::deleteAllEffects()
{
	if(psEngine == NULL)	
		return false;

	FOR(i, ESZ(listOfEffects))deleteEffect(i);

	return true;
}