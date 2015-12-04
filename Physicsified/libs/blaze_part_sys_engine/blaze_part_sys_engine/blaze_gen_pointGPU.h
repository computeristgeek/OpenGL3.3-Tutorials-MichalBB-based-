#pragma once

#include "blaze_gen_point.h"
#include "blaze_gen_GPU.h"

class BlazePointParticleGeneratorGPU : public BlazePointParticleGenerator, public BlazeParticleGeneratorGPU
{
public:
	void setSpecificGeneratorParameters();
};
