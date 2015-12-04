#include "common_header.h"

#include "blaze_sys_common.h"

#include "blaze_gen_point.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#include <glm/gtx/rotate_vector.hpp>

BlazeParticleSystemEngineCommon::BlazeParticleSystemEngineCommon()
{
	bInitialized = false;
}

void BlazeParticleSystemEngineCommon::setParticleTexture(unsigned int a_uiTexture)
{
	uiTexture = a_uiTexture;
}

UINT BlazeParticleSystemEngineCommon::addGenerator(BlazeParticleGenerator* gGen)
{
	int iSize = ESZ(gGenerators);

	UINT uiResult = UINT(iFirstFreeIndex);

	if(iSize == iFirstFreeIndex)
	{
		gGenerators.push_back(gGen);
		iFirstFreeIndex++;
	}
	else
	{
		gGenerators[iFirstFreeIndex] = gGen;

		int iNewFreeIndex = iSize;
		SFOR(i, iFirstFreeIndex, iSize-1)
		if(gGenerators[i] == NULL)
		{
			iNewFreeIndex = i;
			break;
		}

		iFirstFreeIndex = iNewFreeIndex;
		uiResult = UINT(iNewFreeIndex);
	}

	return uiResult;
}

bool BlazeParticleSystemEngineCommon::setGeneratorColor(UINT uiGeneratorID, BYTE r, BYTE g, BYTE b)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	gGenerators[uiGeneratorID]->setGeneratorColor(r, g, b);

	return true;
}

bool BlazeParticleSystemEngineCommon::setGeneratorGravityVector(UINT uiGeneratorID, float* fGravityVector)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	gGenerators[uiGeneratorID]->setGravityVector(fGravityVector);

	return true;
}

bool BlazeParticleSystemEngineCommon::setGeneratorSpawnRate(UINT uiGeneratorID, float fNewSpawnEveryMin, float fNewSpawnEveryMax, int iNewMinCount, int iNewMaxCount)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	gGenerators[uiGeneratorID]->setSpawnRate(fNewSpawnEveryMin, fNewSpawnEveryMax, iNewMinCount, iNewMaxCount);

	return true;
}

bool BlazeParticleSystemEngineCommon::setGeneratorParticleSize(UINT uiGeneratorID, float fNewParticleSize)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	gGenerators[uiGeneratorID]->setParticleSize(fNewParticleSize);

	return true;
}

bool BlazeParticleSystemEngineCommon::setSphericalGeneratorCenter(UINT uiGeneratorID, float* fNewPosition)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	BlazeSphericalParticleGenerator* gGen = dynamic_cast<BlazeSphericalParticleGenerator*>(gGenerators[uiGeneratorID]);
	gGen->setPosition(fNewPosition);

	return true;
}

bool BlazeParticleSystemEngineCommon::setSphericalGeneratorRadius(UINT uiGeneratorID, float fNewRadius)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	if(gGenerators[uiGeneratorID]->getGeneratorType() != BLAZE_PART_GEN_TYPE_SPHERE)
		return false;

	BlazeSphericalParticleGenerator* gGen = dynamic_cast<BlazeSphericalParticleGenerator*>(gGenerators[uiGeneratorID]);
	gGen->setSphereRadius(fNewRadius);

	return true;
}

bool BlazeParticleSystemEngineCommon::setBoxGeneratorCorners(UINT uiGeneratorID, float* fNewCornerVertexA, float* fNewCornerVertexB)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	if(gGenerators[uiGeneratorID]->getGeneratorType() != BLAZE_PART_GEN_TYPE_BOX)
		return false;

	BlazeBoxParticleGenerator* gGen = dynamic_cast<BlazeBoxParticleGenerator*>(gGenerators[uiGeneratorID]);
	gGen->setBoxOppositeCorners(fNewCornerVertexA, fNewCornerVertexB);

	return true;
}

bool BlazeParticleSystemEngineCommon::setEllipsoidalGeneratorCenter(UINT uiGeneratorID, float* fNewPosition)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	if(gGenerators[uiGeneratorID]->getGeneratorType() != BLAZE_PART_GEN_TYPE_ELLIPSE)
		return false;

	BlazeEllipsoidalParticleGenerator* gGen = dynamic_cast<BlazeEllipsoidalParticleGenerator*>(gGenerators[uiGeneratorID]);
	gGen->setPosition(fNewPosition);

	return true;
}

bool BlazeParticleSystemEngineCommon::setEllipsoidalGeneratorRadii(UINT uiGeneratorID, float fNewRadiusX, float fNewRadiusY, float fNewRadiusZ)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	if(gGenerators[uiGeneratorID]->getGeneratorType() != BLAZE_PART_GEN_TYPE_ELLIPSE)
		return false;

	BlazeEllipsoidalParticleGenerator* gGen = dynamic_cast<BlazeEllipsoidalParticleGenerator*>(gGenerators[uiGeneratorID]);
	gGen->setEllipseRadii(fNewRadiusX, fNewRadiusY, fNewRadiusZ);

	return true;
}

void BlazeParticleSystemEngineCommon::setLookAt(float fEyeX, float fEyeY, float fEyeZ, float fViewX, float fViewY, float fViewZ, float fUpX, float fUpY, float fUpZ)
{
	mView = glm::lookAt(glm::vec3(fEyeX, fEyeY, fEyeZ), glm::vec3(fViewX, fViewY, fViewZ), glm::vec3(fUpX, fUpY, fUpZ));
	vViewVector = glm::vec3(fViewX, fViewY, fViewZ)-glm::vec3(fEyeX, fEyeY, fEyeZ);
	vViewVector = glm::normalize(vViewVector);
	vQuad1 = glm::cross(vViewVector, glm::vec3(0, 1.0f, 0));
	if(fabsf(vViewVector.y) < 0.0001f)vQuad1 = glm::cross(vViewVector, glm::vec3(1.0f, 0.0f, 0.0f));
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(vViewVector, vQuad1);
	vQuad2 = glm::normalize(vQuad2);
}

bool BlazeParticleSystemEngineCommon::setPointGeneratorPosition(UINT uiGeneratorID, float* fNewPositon)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	if(gGenerators[uiGeneratorID]->getGeneratorType() != BLAZE_PART_GEN_TYPE_POINT)
		return false;

	BlazePointParticleGenerator* pGen = dynamic_cast<BlazePointParticleGenerator*>(gGenerators[uiGeneratorID]);
	pGen->setPosition(fNewPositon);

	return true;
}

void BlazeParticleSystemEngineCommon::setProjectionMatrix(float* a_mProj)
{
	memcpy(&mProj, a_mProj, sizeof(float)*16);
}
