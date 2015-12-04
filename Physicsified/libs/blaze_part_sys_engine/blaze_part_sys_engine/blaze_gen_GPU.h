#pragma once

#include "blaze_gen.h"
#include "blaze_particle.h"
#include "shaders.h"

class BlazeParticleGPU : public BlazeParticle
{
public:
	int iType;
};

class BlazeParticleGeneratorGPU : virtual public BlazeParticleGenerator
{
public:
	void initGPUResources();

	void useProgram();
	virtual void setSpecificGeneratorParameters() = 0;
	void generateParticles(float fTimeElapsed);
	void renderParticles();
	void setShaderPrograms(CShaderProgram* a_spGeomGen, CShaderProgram* a_spRender);
	void setQueryID(UINT a_uiQuery);
	int getNumParticles();

	void requestDelete();
	bool canBeDeleted();
	bool isDeleteRequested();
	void deInitGenerator();
	
	void clearAllParticles();

protected:
	UINT uiPartBuf[2];
	UINT uiVAO[2];
	UINT uiQuery;

	CShaderProgram* spRender, *spGeomGen;

	int iNumParticles;
	int iCurReadBuffer;
	float fTotalTimeElapsed;
	bool bDeleteRequested;
};
