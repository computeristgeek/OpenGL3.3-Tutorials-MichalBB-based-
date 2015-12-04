#include "common_header.h"

#include "dirLight.h"

CDirectionalLight::CDirectionalLight()
{
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);
	vDirection = glm::vec3(0.0f, -1.0f, 0.0f);

	fAmbient = 0.25f;
	iSkybox = 0;
}

CDirectionalLight::CDirectionalLight(glm::vec3 a_vColor, glm::vec3 a_vDirection, GLfloat a_fAmbient, GLint a_iSkybox)
{
	vColor = a_vColor;
	vDirection = a_vDirection;

	fAmbient = a_fAmbient;
	iSkybox = a_iSkybox;
}

/*-----------------------------------------------

Name:	SetUniformData

Params:	spProgram - shader program
		sLightVarName - name of directional light variable

Result:	Sets all directional light data.

/*---------------------------------------------*/

GLvoid CDirectionalLight::SetUniformData(CShaderProgram *spProgram, std::string sLightVarName)
{
	spProgram->SetUniform(sLightVarName+".vColor", vColor);
	spProgram->SetUniform(sLightVarName+".vDirection", vDirection);

	spProgram->SetUniform(sLightVarName+".fAmbient", fAmbient);
	spProgram->SetUniform(sLightVarName+".iSkybox", iSkybox);
}