#include "common_header.h"

#include "blaze_gen_sphericalGPU.h"

void BlazeSphericalParticleGeneratorGPU::setSpecificGeneratorParameters()
{
	glm::vec3* vUpload = (glm::vec3*)fPosition;
	spGeomGen->setUniform("vGenPosition", vUpload);
	spGeomGen->setUniform("fRadius", fRadius);
}