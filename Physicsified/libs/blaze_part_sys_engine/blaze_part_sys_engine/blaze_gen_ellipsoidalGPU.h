#pragma once

#include "blaze_gen_ellipsoidal.h"
#include "blaze_gen_GPU.h"

class BlazeEllipsoidalParticleGeneratorGPU : public BlazeEllipsoidalParticleGenerator, public BlazeParticleGeneratorGPU
{
public:
	void setSpecificGeneratorParameters();
};