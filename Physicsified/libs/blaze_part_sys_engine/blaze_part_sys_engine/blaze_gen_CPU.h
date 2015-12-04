#pragma once

#include "blaze_gen.h"

class BlazeParticleGeneratorCPU : virtual public BlazeParticleGenerator
{
public:
	virtual void generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles) = 0;
};

