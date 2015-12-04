#include "common_header.h"

#include "pointLight.h"

CPointLight::CPointLight()
{
	vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);

	fAmbient = 0.1f;

	fConstantAtt = 0.3f;
	fLinearAtt = 0.007f;
	fExpAtt = 0.00008f;
}

CPointLight::CPointLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, GLfloat a_fAmbient, GLfloat a_fConstantAtt, GLfloat a_fLinearAtt, GLfloat a_fExpAtt)
{
	vColor = a_vColor;
	vPosition = a_vPosition;

	fAmbient = a_fAmbient;
	
	fConstantAtt = a_fConstantAtt;
	fLinearAtt = a_fLinearAtt;
	fExpAtt = a_fExpAtt;
}

/*-----------------------------------------------

Name:	SetUniformData

Params:	spProgram - shader program
		sLightVarName - name of poGLint light variable

Result:	Sets all poGLint light data.

/*---------------------------------------------*/

GLvoid CPointLight::SetUniformData(CShaderProgram* spProgram, string sLightVarName)
{
	spProgram->SetUniform(sLightVarName+".vColor", vColor);
	spProgram->SetUniform(sLightVarName+".vPosition", vPosition);

	spProgram->SetUniform(sLightVarName+".fAmbient", fAmbient);
	
	spProgram->SetUniform(sLightVarName+".fConstantAtt", fConstantAtt);
	spProgram->SetUniform(sLightVarName+".fLinearAtt", fLinearAtt);
	spProgram->SetUniform(sLightVarName+".fExpAtt", fExpAtt);
}

