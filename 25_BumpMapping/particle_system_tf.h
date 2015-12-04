#pragma once

#include "shaders.h"

#define NUM_PARTICLE_ATTRIBUTES 6
#define MAX_PARTICLES_ON_SCENE 100000

#define PARTICLE_TYPE_GENERATOR 0
#define PARTICLE_TYPE_NORMAL 1

/*****************************************************

Class:		CParticle

Purpose:	Encapsulates particle and its properties.

*****************************************************/

class CParticle
{
public:
	glm::vec3 vPosition;
	glm::vec3 vVelocity;
	glm::vec3 vColor;
	GLfloat fLifeTime;
	GLfloat fSize;
	GLint iType;
};

/**********************************************************************

Class:		CParticleSystemTransformFeedback

Purpose:	Particle system class that uses transform feedback feature.

***********************************************************************/

class CParticleSystemTransformFeedback
{
public:
	GLboolean InitalizeParticleSystem();
	GLboolean ReleaseParticleSystem();

	GLvoid RenderParticles();
	GLvoid UpdateParticles(GLfloat fTimePassed);

	GLvoid SetGeneratorProperties(glm::vec3 a_vGenPosition, glm::vec3 a_vGenVelocityMin, glm::vec3 a_vGenVelocityMax, glm::vec3 a_vGenGravityVector, glm::vec3 a_vGenColor, GLfloat a_fGenLifeMin, GLfloat a_fGenLifeMax, GLfloat a_fGenSize, GLfloat fEvery, GLint a_iNumToGenerate);
	GLvoid SetGeneratorPosition(glm::vec3 a_vGenPosition);
	GLvoid SetNumberToGenerate(GLint a_iNumToGenerate);

	GLvoid ClearAllParticles();

	GLint GetNumParticles();

	GLvoid SetMatrices(glm::mat4* a_matProjection, glm::vec3 vEye, glm::vec3 vView, glm::vec3 vUpVector);

	CParticleSystemTransformFeedback();

private:
	GLboolean bInitialized;
	
	GLuint uiTransformFeedbackBuffer;

	GLuint uiParticleBuffer[2];
	GLuint uiVAO[2];

	GLuint uiQuery;
	GLuint uiTexture;

	GLint iCurReadBuffer;
	GLint iNumParticles;

	glm::mat4 matProjection, matView;
	glm::vec3 vQuad1, vQuad2;

	GLfloat fElapsedTime;
	GLfloat fNextGenerationTime;

	glm::vec3 vGenPosition;
	glm::vec3 vGenVelocityMin, vGenVelocityRange;
	glm::vec3 vGenGravityVector;
	glm::vec3 vGenColor;

	GLfloat fGenLifeMin, fGenLifeRange;
	GLfloat fGenSize;

	GLint iNumToGenerate;

	CShader shVertexRender, shGeomRender, shFragRender;
	CShader shVertexUpdate, shGeomUpdate, shFragUpdate;
	CShaderProgram spRenderParticles;
	CShaderProgram spUpdateParticles;
};