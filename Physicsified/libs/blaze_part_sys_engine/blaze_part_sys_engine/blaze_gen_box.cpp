#include "common_header.h"

#include "blaze_gen_box.h"

BlazeBoxParticleGenerator::BlazeBoxParticleGenerator()
{
	iGenType = BLAZE_PART_GEN_TYPE_BOX;
}

void BlazeBoxParticleGenerator::setBoxOppositeCorners(float *a_fCornerVertexA, float *a_fCornerVertexB)
{
	memcpy(fCornerVertexA, a_fCornerVertexA, sizeof(float)*3);
	memcpy(fCornerVertexB, a_fCornerVertexB, sizeof(float)*3);

	FOR(k, 3)fCornerDif[k] = fCornerVertexB[k]-fCornerVertexA[k];
}
