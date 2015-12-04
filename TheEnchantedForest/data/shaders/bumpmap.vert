#version 330

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;                                                                           
	mat4 normalMatrix;
} matrices;
uniform mat4 DepthBiasMVP;

#include "dirLight.frag"

uniform DirectionalLight sunLight;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

smooth out vec2 vTexCoord;
smooth out vec3 vNormal;
smooth out vec3 vWorldPos;
smooth out vec4 vEyeSpacePos;

out vec3 LightDirection_tangentspace;

smooth out vec4 ShadowCoord;


void main()
{
	mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;

   vEyeSpacePos = matrices.viewMatrix*matrices.modelMatrix*vec4(inPosition, 1.0);
	gl_Position = mMVP*vec4(inPosition, 1.0);
	vTexCoord = inCoord;

	vec3 vNormalTransformed = normalize((matrices.normalMatrix*vec4(inNormal, 1.0)).xyz);
	vec3 vTangentTransformed = normalize((matrices.normalMatrix*vec4(inTangent, 1.0)).xyz);
	vec3 vBitangentTransformed = normalize((matrices.normalMatrix*vec4(inBitangent, 1.0)).xyz);
  

	vNormal = vNormalTransformed;
	vWorldPos = (matrices.modelMatrix*vec4(inPosition, 1.0)).xyz;

	mat3 MV3x3 = mat3(matrices.modelMatrix);

	vec3 vertexNormal_cameraspace = MV3x3 * vNormalTransformed;
	vec3 vertexTangent_cameraspace = MV3x3 * vTangentTransformed;
	vec3 vertexBitangent_cameraspace = MV3x3 * vBitangentTransformed;
  
     mat3 TBN = transpose(mat3(
        vertexTangent_cameraspace,
        vertexBitangent_cameraspace,
        vertexNormal_cameraspace
    ));

	 LightDirection_tangentspace = TBN * sunLight.vDirection;

  vec4 vertexPosition_modelspace = matrices.modelMatrix*vec4(inPosition, 1.0);

	ShadowCoord = DepthBiasMVP * vertexPosition_modelspace;


}