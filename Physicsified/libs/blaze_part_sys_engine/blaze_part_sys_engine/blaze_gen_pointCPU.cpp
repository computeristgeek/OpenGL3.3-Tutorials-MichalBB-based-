#include "common_header.h"

#include "blaze_gen_pointCPU.h"

void BlazePointParticleGeneratorCPU::generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles)
{
	fPassedTime += fTimeElapsed;
	if(fPassedTime > fNextTime)
	{
		FOR(j, iNewCount)
		{
			BlazeParticle pNew;
			memcpy(pNew.fPosition, fPosition, sizeof(float)*3);
			FOR(k, 3)
			{
				pNew.fVelocity[k] = grandf(fVelocityRangeMin[k], fVelocityRangeDif[k]);
				pNew.fGravityVector[k] = fGravityVector[k];
			}

			pNew.fLifeTimeSpan = grandf(fLifeMin, fLifeDif);
			pNew.fSize = fParticleSizes;

			memcpy(pNew.fColor, fColor, sizeof(float)*3);

			vParticles->push_back(pNew);
		}
		regenerateNextTime();
	}
}