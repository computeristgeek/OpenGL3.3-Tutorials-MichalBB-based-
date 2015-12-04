#pragma once

#ifndef _AFXDLL
#define _AFXDLL
#endif

#if defined(DLLEXPORT) // inside DLL
#   define BLAZE_EFFECTS_API   __declspec(dllexport)
#else // outside DLL
#   define BLAZE_EFFECTS_API   __declspec(dllimport)
#endif

#include "blaze_part_sys_engine.h"

namespace BlazeEffects
{
	BLAZE_EFFECTS_API void setParticleSystemEngine(BlazeParticleSystemEngine* a_psEngine);

	BLAZE_EFFECTS_API UINT createFire(float* fFireCenter, float* fFireSize, BYTE r, BYTE g, BYTE b);
	BLAZE_EFFECTS_API UINT createFireRing(float* fCenter, float fRadius, int iNumGenerators, BYTE r, BYTE g, BYTE b);
	BLAZE_EFFECTS_API UINT createFireGrid(glm::vec3 fGridCenter, float fGridSize, int iSubDivisions, BYTE r, BYTE g, BYTE b);

	BLAZE_EFFECTS_API bool deleteEffect(UINT uiEffectID);

	BLAZE_EFFECTS_API bool deleteAllEffects();
};