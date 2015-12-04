#include "common_header.h"

#include "blaze_gen_boxCPU.h"

void BlazeBoxParticleGeneratorCPU::generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles)
{
	fPassedTime += fTimeElapsed;
	if(fPassedTime > fNextTime)
	{
		FOR(j, BlazeParticleGenerator::iNewCount)
		{
			BlazeParticle pNew;
			FOR(k, 3)
			{
				pNew.fPosition[k] = grandf(fCornerVertexA[k], fCornerDif[k]);
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
