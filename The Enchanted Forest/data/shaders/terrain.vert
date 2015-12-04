#version 330

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;                                                                           
	mat4 normalMatrix;
} matrices;

uniform mat4 DepthBiasMVP;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

smooth out vec2 vTexCoord;
smooth out vec3 vNormal;
smooth out vec3 vWorldPos;
smooth out vec4 vEyeSpacePos;

smooth out vec4 ShadowCoord;



void main()
{
	mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;

	gl_Position = mMVP*vec4(inPosition, 1.0);
  vEyeSpacePos = matrices.viewMatrix*matrices.modelMatrix*vec4(inPosition, 1.0);
	vTexCoord = inCoord;
	vNormal = inNormal;
  vec4 vWorldPosLocal = matrices.modelMatrix*vec4(inPosition, 1.0);
	vWorldPos = vWorldPosLocal.xyz;
  
  ShadowCoord = DepthBiasMVP * vWorldPosLocal;
}