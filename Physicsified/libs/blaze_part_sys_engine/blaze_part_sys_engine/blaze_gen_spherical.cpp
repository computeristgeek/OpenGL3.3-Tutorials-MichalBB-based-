#include "common_header.h"

#include "blaze_gen_spherical.h"

BlazeSphericalParticleGenerator::BlazeSphericalParticleGenerator()
{
	iGenType = BLAZE_PART_GEN_TYPE_SPHERE;
}

void BlazeSphericalParticleGenerator::setPosition(float* a_fPosition)
{
	memcpy(fPosition, a_fPosition, sizeof(float)*3);
}

void BlazeSphericalParticleGenerator::setSphereRadius(float a_fRadius)
{
	fRadius = a_fRadius;
}