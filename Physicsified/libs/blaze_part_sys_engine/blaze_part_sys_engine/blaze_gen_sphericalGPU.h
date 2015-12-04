#pragma once

#include "blaze_gen_spherical.h"
#include "blaze_gen_GPU.h"

class BlazeSphericalParticleGeneratorGPU : public BlazeSphericalParticleGenerator, public BlazeParticleGeneratorGPU
{
public:
	void setSpecificGeneratorParameters();
};