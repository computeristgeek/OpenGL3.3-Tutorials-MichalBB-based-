#pragma once

#include "shaders.h"

#define FOG_EQUATION_LINEAR	0
#define FOG_EQUATION_EXP	1
#define FOG_EQUATION_EXP2	2

/********************************

Class:	CFogParameters

Purpose: Support class for managing
		fog in the scene.

********************************/

class CFogParameters
{
public:
	glm::vec4 vFogColor;
	
	GLfloat fStart;
	GLfloat fEnd;
	GLfloat fDensity;

	GLint iEquation;

	GLvoid setUniformData(CShaderProgram* spProgram, string sFogVarName);

	CFogParameters();
	CFogParameters(glm::vec4 a_vFogColor, GLfloat a_fStart, GLfloat a_fEnd, GLfloat a_fDensity, GLint a_iEquation);
};