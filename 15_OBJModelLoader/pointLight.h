#pragma once

#include "shaders.h"

/********************************

Class:	CPointLight

Purpose: Support class for adding
		point lights to scene.

********************************/

class CPointLight
{
public:
	glm::vec3 vColor; // Color of point light
	glm::vec3 vPosition; // and position

	GLfloat fAmbient;

	GLfloat fConstantAtt;
	GLfloat fLinearAtt;
	GLfloat fExpAtt;

	GLvoid setUniformData(CShaderProgram* spProgram, string sLightVarName);

	CPointLight();
	CPointLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, GLfloat a_fAmbient, GLfloat a_fConstantAtt, GLfloat a_fLinearAtt, GLfloat a_fExpAtt);
};