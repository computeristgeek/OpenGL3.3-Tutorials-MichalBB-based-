#pragma once

#include "blaze_gen.h"

class BlazePointParticleGenerator : virtual public BlazeParticleGenerator
{
public:
	void setPosition(float* a_fPosition);

	BlazePointParticleGenerator();
protected:
	float fPosition[3];
};