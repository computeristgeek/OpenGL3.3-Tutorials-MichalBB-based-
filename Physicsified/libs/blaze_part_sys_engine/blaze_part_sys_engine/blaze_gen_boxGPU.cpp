#include "common_header.h"

#include "blaze_gen_boxGPU.h"

void BlazeBoxParticleGeneratorGPU::setSpecificGeneratorParameters()
{
	glm::vec3* vUpload = (glm::vec3*)fCornerVertexA;
	spGeomGen->setUniform("vGenCornerA", vUpload);
	vUpload = (glm::vec3*)fCornerDif;
	spGeomGen->setUniform("vGenCornerDif", vUpload);
}