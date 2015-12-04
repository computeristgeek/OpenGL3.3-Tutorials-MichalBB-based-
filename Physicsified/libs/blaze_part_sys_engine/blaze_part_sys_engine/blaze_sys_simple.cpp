#include "common_header.h"

#include "blaze_sys_simple.h"

void BlazeSimpleParticleSystemEngine::renderParticles()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(0);

	glBegin(GL_QUADS);

	FOR(i, ESZ(pParticles))
	{
		float fAlpha = min(1.0f, pParticles[i].fLifeTimeSpan);
		float& fPartSize = pParticles[i].fSize;
		glColor4f(pParticles[i].fColor[0], pParticles[i].fColor[1], pParticles[i].fColor[2], fAlpha);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pParticles[i].fPosition[0]+(-vQuad1.x+vQuad2.x)*fPartSize, pParticles[i].fPosition[1]+(-vQuad1.y+vQuad2.y)*fPartSize, pParticles[i].fPosition[2]+(-vQuad1.z+vQuad2.z)*fPartSize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pParticles[i].fPosition[0]+(vQuad1.x+vQuad2.x)*fPartSize, pParticles[i].fPosition[1]+(vQuad1.y+vQuad2.y)*fPartSize, pParticles[i].fPosition[2]+(vQuad1.z+vQuad2.z)*fPartSize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pParticles[i].fPosition[0]+(vQuad1.x-vQuad2.x)*fPartSize, pParticles[i].fPosition[1]+(vQuad1.y-vQuad2.y)*fPartSize, pParticles[i].fPosition[2]+(vQuad1.z-vQuad2.z)*fPartSize);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pParticles[i].fPosition[0]+(-vQuad1.x-vQuad2.x)*fPartSize, pParticles[i].fPosition[1]+(-vQuad1.y-vQuad2.y)*fPartSize, pParticles[i].fPosition[2]+(-vQuad1.z-vQuad2.z)*fPartSize);
	}
	glEnd();
	glDepthMask(1);

	glDisable(GL_BLEND);
}

BLAZE_PART_SYS_ENGINE_API BlazeParticleSystemEngine* createSimpleParticleSystem()
{
	BlazeParticleSystemEngine* psEngine = new BlazeSimpleParticleSystemEngine;
	return psEngine;
}