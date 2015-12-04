#include "common_header.h"

#include "blaze_sys_transformfeedback.h"

bool BlazeTransformFeedbackParticleSystemEngine::initalizeParticleSystem(void* initData)
{
	if(bInitialized)return false;

	iFirstFreeIndex = 0;
	gGenerators.reserve(400);

	shVertexGen.loadShader("data\\shaders\\blaze_part_gen.vert", GL_VERTEX_SHADER);
	shGeomGenPoint.loadShader("data\\shaders\\blaze_part_gen_point.geom", GL_GEOMETRY_SHADER);
	shGeomGenSpherical.loadShader("data\\shaders\\blaze_part_gen_sphere.geom", GL_GEOMETRY_SHADER);
	shGeomGenBox.loadShader("data\\shaders\\blaze_part_gen_box.geom", GL_GEOMETRY_SHADER);
	shGeomGenEllipsoidal.loadShader("data\\shaders\\blaze_part_gen_ellipse.geom", GL_GEOMETRY_SHADER);

	const char* sVaryings[7] = 
	{
		"vPositionOut",
		"vVelocityOut",
		"vGravityOut",
		"vColorOut",
		"fLifeTimeOut",
		"fSizeOut",
		"iTypeOut"
	};

	spProgramGenPoint.createProgram();
	spProgramGenPoint.addShaderToProgram(&shVertexGen);
	spProgramGenPoint.addShaderToProgram(&shGeomGenPoint);
	glTransformFeedbackVaryings(spProgramGenPoint.getProgramID(), 7, sVaryings, GL_INTERLEAVED_ATTRIBS);
	spProgramGenPoint.linkProgram();
/*
	char log[1024];
	int len;
	glGetProgramInfoLog(spProgramGenPoint.getProgramID(), 1024, &len, log);

	MessageBox(NULL, log, "FSDFDFD", MB_OK);
*/
	spProgramGenSpherical.createProgram();
	spProgramGenSpherical.addShaderToProgram(&shVertexGen);
	spProgramGenSpherical.addShaderToProgram(&shGeomGenSpherical);
	glTransformFeedbackVaryings(spProgramGenSpherical.getProgramID(), 7, sVaryings, GL_INTERLEAVED_ATTRIBS);
	spProgramGenSpherical.linkProgram();

	spProgramGenBox.createProgram();
	spProgramGenBox.addShaderToProgram(&shVertexGen);
	spProgramGenBox.addShaderToProgram(&shGeomGenBox);

	glTransformFeedbackVaryings(spProgramGenBox.getProgramID(), 7, sVaryings, GL_INTERLEAVED_ATTRIBS);
	spProgramGenBox.linkProgram();
	spProgramGenEllipsoidal.createProgram();
	spProgramGenEllipsoidal.addShaderToProgram(&shVertexGen);
	spProgramGenEllipsoidal.addShaderToProgram(&shGeomGenEllipsoidal);
	glTransformFeedbackVaryings(spProgramGenEllipsoidal.getProgramID(), 7, sVaryings, GL_INTERLEAVED_ATTRIBS);
	spProgramGenEllipsoidal.linkProgram();

	shVertexRender.loadShader("data\\shaders\\blaze_part_render.vert", GL_VERTEX_SHADER);
	shGeomRender.loadShader("data\\shaders\\blaze_part_render.geom", GL_GEOMETRY_SHADER);
	shFragRender.loadShader("data\\shaders\\blaze_part_render.frag", GL_FRAGMENT_SHADER);
	
	spProgramRender.createProgram();

	spProgramRender.addShaderToProgram(&shVertexRender);
	spProgramRender.addShaderToProgram(&shGeomRender);
	spProgramRender.addShaderToProgram(&shFragRender);

	spProgramRender.linkProgram();

	glGenSamplers(1, &uiSampler);
	glBindSampler(0, uiSampler);
	glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glGenTransformFeedbacks(1, &uiTransFeedbackBuf);
	glGenQueries(1, &uiQuery);

	bInitialized = true;

	return true;
}

UINT BlazeTransformFeedbackParticleSystemEngine::createPointParticleGenerator(float* a_fPosition, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazePointParticleGeneratorGPU* gNew = new BlazePointParticleGeneratorGPU;

	gNew->initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->initGPUResources();
	gNew->setShaderPrograms(&spProgramGenPoint, &spProgramRender);
	gNew->setQueryID(uiQuery);

	gNew->setPosition(a_fPosition);
	
	return addGenerator(gNew);
}

UINT BlazeTransformFeedbackParticleSystemEngine::createSphericalParticleGenerator(float* a_fPosition, float a_fRadius, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazeSphericalParticleGeneratorGPU* gNew = new BlazeSphericalParticleGeneratorGPU;

	gNew->initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->initGPUResources();
	gNew->setShaderPrograms(&spProgramGenSpherical, &spProgramRender);
	gNew->setQueryID(uiQuery);

	gNew->setPosition(a_fPosition);
	gNew->setSphereRadius(a_fRadius);

	return addGenerator(gNew);
}

UINT BlazeTransformFeedbackParticleSystemEngine::createBoxParticleGenerator(float* a_fCornerVertexA, float* a_fCornerVertexB, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazeBoxParticleGeneratorGPU* gNew = new BlazeBoxParticleGeneratorGPU;

	gNew->initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->initGPUResources();
	gNew->setShaderPrograms(&spProgramGenBox, &spProgramRender);
	gNew->setQueryID(uiQuery);

	gNew->setBoxOppositeCorners(a_fCornerVertexA, a_fCornerVertexB);

	return addGenerator(gNew);
}

UINT BlazeTransformFeedbackParticleSystemEngine::createEllipsoidalParticleGenerator(float* a_fPosition, float a_fRadiusX, float a_fRadiusY, float a_fRadiusZ, float* a_fVelocityRangeMin, float* a_fVelocityRangeMax, float a_fSpawnEveryMin, float a_fSpawnEveryMax, int a_countMin, int a_countMax, float a_fLifeMin, float a_fLifeMax, float a_fParticleSizes, int a_maxSpawns)
{
	BlazeEllipsoidalParticleGeneratorGPU* gNew = new BlazeEllipsoidalParticleGeneratorGPU;

	gNew->initGenerator(a_fVelocityRangeMin, a_fVelocityRangeMax, a_fSpawnEveryMin, a_fSpawnEveryMax, a_countMin, a_countMax, a_fLifeMin, a_fLifeMax, a_fParticleSizes, a_maxSpawns);
	gNew->initGPUResources();
	gNew->setShaderPrograms(&spProgramGenEllipsoidal, &spProgramRender);
	gNew->setQueryID(uiQuery);
	
	gNew->setPosition(a_fPosition);
	gNew->setEllipseRadii(a_fRadiusX, a_fRadiusY, a_fRadiusZ);

	return addGenerator(gNew);
}

void BlazeTransformFeedbackParticleSystemEngine::renderParticles()
{
	if(!bInitialized)return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBindSampler(0, uiSampler);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(0);

	glDisable(GL_RASTERIZER_DISCARD);
	spProgramRender.useProgram();
	spProgramRender.setUniform("matrices.mProj", &mProj);
	spProgramRender.setUniform("matrices.mView", &mView);
	spProgramRender.setUniform("vQuad1", &vQuad1);
	spProgramRender.setUniform("vQuad2", &vQuad2);
	spProgramRender.setUniform("gSampler", 0);
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)
	{
		BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[i]);
		gGen->renderParticles();
		if(gGen->isDead())deleteParticleGenerator(i);
	}
	glDepthMask(1);
	glDisable(GL_BLEND);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void BlazeTransformFeedbackParticleSystemEngine::updateParticles(float fTimePassed)
{
	if(!bInitialized)return;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, uiTransFeedbackBuf);

	glEnable(GL_RASTERIZER_DISCARD);
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)
	{
		BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[i]);

		gGen->useProgram();
		gGen->setSpecificGeneratorParameters();
		gGen->generateParticles(fTimePassed);
		if(gGen->canBeDeleted())
		{
			gGen->deInitGenerator();
			delete gGenerators[i];
			gGenerators[i] = NULL;
		}
	}
}

bool BlazeTransformFeedbackParticleSystemEngine::deleteParticleGenerator(UINT uiGeneratorID)
{
	if(uiGeneratorID > UINT(gGenerators.size()))
		return false;

	if(gGenerators[uiGeneratorID] == NULL)
		return false;

	BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[uiGeneratorID]);

	gGen->requestDelete();
	if(gGen->canBeDeleted())
	{
		gGen->deInitGenerator();
		delete gGenerators[uiGeneratorID];
		gGenerators[uiGeneratorID] = NULL;
	}
	return true;
}

int BlazeTransformFeedbackParticleSystemEngine::getNumParticles()
{
	int iRes = 0;
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)
	{
		BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[i]);
		iRes += gGen->getNumParticles();
	}

	return iRes;
}

int BlazeTransformFeedbackParticleSystemEngine::getNumGenerators()
{
	int iRes = 0;
	FOR(i, ESZ(gGenerators))
	{
		if(gGenerators[i] != NULL)
		{
			BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[i]);
			if(!gGen->isDeleteRequested())iRes++;
		}
	}

	return iRes;
}

bool BlazeTransformFeedbackParticleSystemEngine::releaseParticleSystem()
{
	FOR(i, ESZ(gGenerators))
	if(gGenerators[i] != NULL)
	{
		BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[i]);
		gGen->clearAllParticles();
	}
	clearAllGenerators();
	glDeleteTransformFeedbacks(1, &uiTransFeedbackBuf);
	glDeleteQueries(1, &uiQuery);
	glDeleteSamplers(1, &uiSampler);

	return true;
}

void BlazeTransformFeedbackParticleSystemEngine::clearAllParticles()
{
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)
	{
		BlazeParticleGeneratorGPU* gGen = dynamic_cast<BlazeParticleGeneratorGPU*>(gGenerators[i]);
		gGen->clearAllParticles();
	}
}

void BlazeTransformFeedbackParticleSystemEngine::clearAllGenerators()
{
	FOR(i, ESZ(gGenerators))if(gGenerators[i] != NULL)deleteParticleGenerator(i);
}

BLAZE_PART_SYS_ENGINE_API BlazeParticleSystemEngine* createTransformFeedbackParticleSystem()
{
	BlazeParticleSystemEngine* psEngine = new BlazeTransformFeedbackParticleSystemEngine;
	return psEngine;
}

