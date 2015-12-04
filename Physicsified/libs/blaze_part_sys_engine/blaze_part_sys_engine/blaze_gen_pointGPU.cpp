#include "common_header.h"

#include "blaze_gen_pointGPU.h"

void BlazePointParticleGeneratorGPU::setSpecificGeneratorParameters()
{
	glm::vec3* vUpload = (glm::vec3*)fPosition;
	spGeomGen->setUniform("vGenPosition", vUpload);
}