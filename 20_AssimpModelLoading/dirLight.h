#pragma once

#include "shaders.h"

/********************************

Class:		CPointLight

Purpose:	Support class for adding
			directional lights to scene.

********************************/

class CDirectionalLight
{
public:
	glm::vec3 vColor; // Color of directional light
	glm::vec3 vDirection; // and its direction

	GLfloat fAmbient;

	GLvoid SetUniformData(CShaderProgram* spProgram, string sLightVarName);

	CDirectionalLight();
	CDirectionalLight(glm::vec3 a_vColor, glm::vec3 a_vDirection, GLfloat a_fAmbient);
};