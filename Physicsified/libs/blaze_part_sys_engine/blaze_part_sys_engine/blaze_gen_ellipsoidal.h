#pragma once

#include "blaze_gen.h"

class BlazeEllipsoidalParticleGenerator : virtual public BlazeParticleGenerator
{
public:
	void setPosition(float* a_fPosition);
	void setEllipseRadii(float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ);

	BlazeEllipsoidalParticleGenerator();
protected:
	float fPosition[3];
	float fRadiusX, fRadiusY, fRadiusZ;
};