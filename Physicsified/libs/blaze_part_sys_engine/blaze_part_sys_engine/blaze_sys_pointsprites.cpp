#include "common_header.h"

#include "blaze_sys_pointsprites.h"

void BlazePointSpritesParticleSystemEngine::renderParticles()
{
	glDepthMask(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, uiTexture);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	float quadratic[] =  { -1.0f, -0.1f, -0.01f };
	glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

	// The alpha of a point is calculated to allow the fading of points 
	// instead of shrinking them past a defined threshold size. The threshold 
	// is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
	// the minimum and maximum point sizes.
	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 1.0f );

	glPointParameterf( GL_POINT_SIZE_MIN_ARB, 20.0f );
	glPointParameterf( GL_POINT_SIZE_MAX_ARB, 60.0f );

	// Specify point sprite texture coordinate replacement mode for each texture unit
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );

	glEnable( GL_POINT_SPRITE_ARB );

	float fLastSize = 0.0f;
	bool bBegun = false;

	FOR(i, ESZ(pParticles))
	{
		if(pParticles[i].fSize != fLastSize)
		{
			if(bBegun)
			{
				glEnd();
				bBegun = false;
				glPointSize(pParticles[i].fSize);
				fLastSize = pParticles[i].fSize;
			}
		}
		float fAlpha = min(1.0f, pParticles[i].fLifeTimeSpan);
		float& fPartSize = pParticles[i].fSize;
		if(!bBegun)
		{
			glBegin(GL_POINTS);
			bBegun = true;
		}
		glColor4f(pParticles[i].fColor[0], pParticles[i].fColor[1], pParticles[i].fColor[2], fAlpha);
		glVertex3fv(pParticles[i].fPosition);
	}
	if(bBegun)glEnd();

	glDepthMask(1);
	glDisable(GL_BLEND);
}

BLAZE_PART_SYS_ENGINE_API BlazeParticleSystemEngine* createPointSpritesParticleSystem()
{
	BlazeParticleSystemEngine* psEngine = new BlazePointSpritesParticleSystemEngine;
	return psEngine;
}
