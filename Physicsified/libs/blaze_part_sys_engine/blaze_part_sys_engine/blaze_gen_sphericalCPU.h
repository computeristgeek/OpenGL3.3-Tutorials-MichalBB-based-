#pragma once

#include "blaze_gen_spherical.h"
#include "blaze_gen_CPU.h"

class BlazeSphericalParticleGeneratorCPU : public BlazeSphericalParticleGenerator, public BlazeParticleGeneratorCPU
{
public:
	void generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles);
};