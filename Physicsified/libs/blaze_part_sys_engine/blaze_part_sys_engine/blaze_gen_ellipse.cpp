#include "common_header.h"

#include "blaze_gen_ellipsoidal.h"

BlazeEllipsoidalParticleGenerator::BlazeEllipsoidalParticleGenerator()
{
	iGenType = BLAZE_PART_GEN_TYPE_ELLIPSE;
}

void BlazeEllipsoidalParticleGenerator::setPosition(float* a_fPosition)
{
	memcpy(fPosition, a_fPosition, sizeof(float)*3);
}

void BlazeEllipsoidalParticleGenerator::setEllipseRadii(float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ)
{
	fRadiusX = a_fRadiusX;
	fRadiusY = a_fRadiusY;
	fRadiusZ = a_fRadiusZ;
}