#pragma once

#define DLLEXPORT

#include "blaze_sys_cpu.h"

class BlazePointSpritesParticleSystemEngine : public BlazeParticleSystemEngineCPU
{
public:
	void renderParticles();
};