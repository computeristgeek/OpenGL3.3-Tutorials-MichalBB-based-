#pragma once

#include "shaders.h"

/********************************

Class:		CSpotLight

Purpose:	Support class for adding
			spotlights to scene.

********************************/

class CSpotLight
{
public:
	glm::vec3 vColor;
	glm::vec3 vPosition;
	glm::vec3 vDirection;

	GLint bOn;
	GLfloat fConeAngle;
	GLfloat fLinearAtt;

	GLvoid SetUniformData(CShaderProgram* spProgram, string sLightVarName);

	CSpotLight();
	CSpotLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, glm::vec3 a_vDirection, GLint a_bOn, GLfloat a_fConeAngle, GLfloat a_fLinearAtt);
private:
	// This shouldn't be changed from outside
	GLfloat fConeCosine;
};