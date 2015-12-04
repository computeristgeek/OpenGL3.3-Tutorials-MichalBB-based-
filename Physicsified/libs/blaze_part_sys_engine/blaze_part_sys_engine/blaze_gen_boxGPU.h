#pragma once

#include "blaze_gen_box.h"
#include "blaze_gen_GPU.h"

class BlazeBoxParticleGeneratorGPU : public BlazeBoxParticleGenerator, public BlazeParticleGeneratorGPU
{
public:
	void setSpecificGeneratorParameters();
};