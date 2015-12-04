#pragma once

#define DLLEXPORT

#include "blaze_sys_cpu.h"

class BlazeSimpleParticleSystemEngine : public BlazeParticleSystemEngineCPU
{
public:
	void renderParticles();
};