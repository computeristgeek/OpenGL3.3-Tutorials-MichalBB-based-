#pragma once

#include "blaze_gen.h"

class BlazeBoxParticleGenerator : virtual public BlazeParticleGenerator
{
public:
	void setBoxOppositeCorners(float* a_fCornerVertexA, float* a_fCornerVertexB);

	BlazeBoxParticleGenerator();
protected:
	float fCornerVertexA[3], fCornerVertexB[3];
	float fCornerDif[3];
};