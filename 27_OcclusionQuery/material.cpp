
#include "common_header.h"

#include "material.h"

CMaterial::CMaterial()
{
	fSpecularIntensity = 1.0f;
	fSpecularPower = 32.0f;
}

CMaterial::CMaterial(GLfloat a_fSpecularIntensity, GLfloat a_fSpecularPower)
{
	fSpecularIntensity = a_fSpecularIntensity;
	fSpecularPower = a_fSpecularPower;
}

/*-----------------------------------------------

Name:	SetUniformData

Params:	spProgram - shader program
		sMaterialVarName - name of material variable

Result:	Sets all material uniform data.

/*---------------------------------------------*/

GLvoid CMaterial::SetUniformData(CShaderProgram* spProgram, string sMaterialVarName)
{
	spProgram->SetUniform(sMaterialVarName+".fSpecularIntensity", fSpecularIntensity);
	spProgram->SetUniform(sMaterialVarName+".fSpecularPower", fSpecularPower);
}