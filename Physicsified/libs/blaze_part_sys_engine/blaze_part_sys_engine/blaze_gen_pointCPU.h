#pragma once

#include "blaze_gen_point.h"
#include "blaze_gen_CPU.h"

class BlazePointParticleGeneratorCPU : public BlazePointParticleGenerator, public BlazeParticleGeneratorCPU
{
public:
	void generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles);
};