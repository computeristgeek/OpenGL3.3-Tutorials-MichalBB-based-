#include "common_header.h"

#include "blaze_gen_ellipsoidalCPU.h"

void BlazeEllipsoidalParticleGeneratorCPU::generateParticles(float fTimeElapsed, vector<BlazeParticle>* vParticles)
{
	fPassedTime += fTimeElapsed;
	if(fPassedTime > fNextTime)
	{
		FOR(j, iNewCount)
		{
			BlazeParticle pNew;

			float fAngle = grandf(0.0f, 2.0f*3.1415f);
			float fVectorA[3] = {float(cos(fAngle)), 0.0f, float(sin(fAngle))};
			float fVectorB[3] = {0.0f, 1.0f ,0.0f};

			fAngle = grandf(0.0f, 2.0f*3.1415f);
			float fSine = float(sin(fAngle));
			float fCosine = float(cos(fAngle));

			float fRandRadius = grandf(0.0f, fRadiusX);

			FOR(k, 3)
			{
				pNew.fPosition[k] = fPosition[k]+(fCosine*fVectorA[k] + fSine*fVectorB[k])*fRandRadius;
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
