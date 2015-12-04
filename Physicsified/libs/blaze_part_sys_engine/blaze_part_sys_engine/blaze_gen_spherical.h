#pragma once

#include "blaze_gen.h"

class BlazeSphericalParticleGenerator : virtual public BlazeParticleGenerator
{
public:
	void setPosition(float* a_fPosition);
	void setSphereRadius(float a_fRadius);

	BlazeSphericalParticleGenerator();
protected:
	float fPosition[3];
	float fRadius;
};