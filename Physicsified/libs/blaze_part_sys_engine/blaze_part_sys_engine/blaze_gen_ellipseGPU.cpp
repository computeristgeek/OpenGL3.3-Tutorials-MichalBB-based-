#include "common_header.h"

#include "blaze_gen_ellipsoidalGPU.h"

void BlazeEllipsoidalParticleGeneratorGPU::setSpecificGeneratorParameters()
{
	glm::vec3* vUpload = (glm::vec3*)fPosition;
	spGeomGen->setUniform("vGenPosition", vUpload);
	spGeomGen->setUniform("fRadiusX", fRadiusX);
	spGeomGen->setUniform("fRadiusY", fRadiusY);
	spGeomGen->setUniform("fRadiusZ", fRadiusZ);
}