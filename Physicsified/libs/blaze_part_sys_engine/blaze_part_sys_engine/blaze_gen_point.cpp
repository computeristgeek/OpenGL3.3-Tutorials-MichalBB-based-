#include "common_header.h"

#include "blaze_gen_point.h"

BlazePointParticleGenerator::BlazePointParticleGenerator()
{
	iGenType = BLAZE_PART_GEN_TYPE_POINT;
}

void BlazePointParticleGenerator::setPosition(float* a_fPosition)
{
	memcpy(fPosition, a_fPosition, sizeof(float)*3);
}