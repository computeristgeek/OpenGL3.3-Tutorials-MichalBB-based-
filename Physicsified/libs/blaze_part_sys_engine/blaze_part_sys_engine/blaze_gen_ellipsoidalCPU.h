#pragma once

#include "blaze_gen_ellipsoidal.h"
#include "blaze_gen_CPU.h"

class BlazeEllipsoidalParticleGeneratorCPU : public BlazeEllipsoidalParticleGenerator, public BlazeParticleGeneratorCPU
{
public:
	void generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles);
};