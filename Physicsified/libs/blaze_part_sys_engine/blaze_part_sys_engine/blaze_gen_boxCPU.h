#pragma once

#include "blaze_gen_box.h"
#include "blaze_gen_CPU.h"

class BlazeBoxParticleGeneratorCPU : public BlazeBoxParticleGenerator, public BlazeParticleGeneratorCPU
{
public:
	void generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles);
};