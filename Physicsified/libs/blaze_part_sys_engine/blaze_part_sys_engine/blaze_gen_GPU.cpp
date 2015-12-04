#include "common_header.h"

#include "blaze_gen_GPU.h"

void BlazeParticleGeneratorGPU::initGPUResources()
{
	glGenBuffers(2, uiPartBuf);
	glGenVertexArrays(2, uiVAO);

	int iMaximumParticles = countMin+int(((fLifeMin+fLifeDif)/fSpawnEveryMin)*float(countMin+countDif));

	FOR(i, 2)
	{
		BlazeParticleGPU initPart;

		//	memset(&initPart, 0, sizeof(BlazeParticle));
	
		initPart.iType = 0;
		initPart.fLifeTimeSpan = 2.0f;
		initPart.fPosition[0] = initPart.fPosition[1] = initPart.fPosition[2] = 0.0f;
		initPart.fSize = 0.0f;

		glBindVertexArray(uiVAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, uiPartBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BlazeParticleGPU)*iMaximumParticles, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BlazeParticleGPU), &initPart);

		FOR(i, 7)glEnableVertexAttribArray(i);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)0); // position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)12); // velocity
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)24); // gravity vector
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)36); // color
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)48); // lifetime
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)52); // size
		glVertexAttribPointer(6, 1, GL_INT,   GL_FALSE, sizeof(BlazeParticleGPU), (const GLvoid*)56); // type
	}
	iCurReadBuffer = 0;
	fTotalTimeElapsed = 0.0f;
	bDeleteRequested = false;
	iNumParticles = 1;

}

void BlazeParticleGeneratorGPU::setQueryID(UINT a_uiQuery)
{
	uiQuery = a_uiQuery;
}

void BlazeParticleGeneratorGPU::setShaderPrograms(CShaderProgram* a_spGeomGen, CShaderProgram* a_spRender)
{
	spGeomGen = a_spGeomGen;
	spRender = a_spRender;
}

void BlazeParticleGeneratorGPU::renderParticles()
{
	glBindVertexArray(uiVAO[iCurReadBuffer]);
	glDisableVertexAttribArray(1); // We don't need velocity for rendering
	glDisableVertexAttribArray(2); // And gravity vector

	glDrawArrays(GL_POINTS, 0, iNumParticles);
}

int BlazeParticleGeneratorGPU::getNumParticles()
{
	return iNumParticles-1;
}

void BlazeParticleGeneratorGPU::requestDelete()
{
	bDeleteRequested = true;
}

bool BlazeParticleGeneratorGPU::canBeDeleted()
{
	return bDeleteRequested && iNumParticles == 1;
}

void BlazeParticleGeneratorGPU::deInitGenerator()
{
	glDeleteVertexArrays(2, uiVAO);
	glDeleteBuffers(2, uiPartBuf);
}

void BlazeParticleGeneratorGPU::clearAllParticles()
{
	iNumParticles = 1;
	BlazeParticleGPU initPart;

	//	memset(&initPart, 0, sizeof(BlazeParticle));

	initPart.iType = 0;
	initPart.fLifeTimeSpan = 2.0f;
	initPart.fPosition[0] = initPart.fPosition[1] = initPart.fPosition[2] = 0.0f;
	initPart.fSize = 0.0f;

	iCurReadBuffer = 0;
	glBindBuffer(GL_ARRAY_BUFFER, uiPartBuf[iCurReadBuffer]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BlazeParticleGPU), &initPart);
}

void BlazeParticleGeneratorGPU::useProgram()
{
	spGeomGen->useProgram();
}

bool BlazeParticleGeneratorGPU::isDeleteRequested()
{
	return bDeleteRequested;
}

void BlazeParticleGeneratorGPU::generateParticles(float fTimeElapsed)
{
	glm::vec3* vUpload;
	spGeomGen->setUniform("fDeltaTime", fTimeElapsed);
	vUpload = (glm::vec3*)fVelocityRangeMin;
	spGeomGen->setUniform("vGenVelocityMin", vUpload);
	vUpload = (glm::vec3*)fVelocityRangeDif;
	spGeomGen->setUniform("vGenVelocityDif", vUpload);
	vUpload = (glm::vec3*)fColor;
	spGeomGen->setUniform("vGenColor", vUpload);
	vUpload = (glm::vec3*)fGravityVector;
	spGeomGen->setUniform("vGenGravityVector", vUpload);

	spGeomGen->setUniform("fLifeMin", fLifeMin);
	spGeomGen->setUniform("fLifeDif", fLifeDif);
	fPassedTime += fTimeElapsed;

	spGeomGen->setUniform("fTimeLeft", fNextTime-fPassedTime);
	spGeomGen->setUniform("iNumToGenerate", iNewCount);
	spGeomGen->setUniform("fGenSize", fParticleSizes);

	fTotalTimeElapsed += fTimeElapsed;
	glm::vec3 vSeed = glm::vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
	spGeomGen->setUniform("vSeed", &vSeed);

	spGeomGen->setUniform("iGenerate", bDeleteRequested ? 0 : 1);

	glBindVertexArray(uiVAO[iCurReadBuffer]);
	glEnableVertexAttribArray(1); // Re-enable velocity
	glEnableVertexAttribArray(2); // And gravity vector

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, uiPartBuf[1-iCurReadBuffer]);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, uiQuery);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, iNumParticles);

	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(uiQuery, GL_QUERY_RESULT, &iNumParticles);

	if(fPassedTime > fNextTime)regenerateNextTime();

	iCurReadBuffer = 1-iCurReadBuffer;
}