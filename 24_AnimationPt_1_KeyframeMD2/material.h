#pragma once

#include "shaders.h"

/********************************

Class:		CMaterial

Purpose:	Support class for handling materials
			in the scene.

********************************/

class CMaterial
{
public:
	GLfloat fSpecularIntensity;
	GLfloat fSpecularPower;

	GLvoid SetUniformData(CShaderProgram* spProgram, string sMaterialVarName);

	CMaterial();
	CMaterial(GLfloat a_fSpecularIntensity, GLfloat a_fSpecularPower);
};