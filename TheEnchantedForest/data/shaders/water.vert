#version 330

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;                                                                           
	mat4 normalMatrix;
} matrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

smooth out vec2 vTexCoord;
smooth out vec3 vNormal;
smooth out vec3 vWorldPos;
smooth out vec4 vEyeSpacePos;

uniform float timePassed;
      
void main()
{
	mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;
  
  vec3 vPositionNoY = inPosition;
  vPositionNoY.y = 0.0f;
  float fLength = length(vPositionNoY);
  
  //const float fHeight = 15;
  //const float fPeriod = 8.0*3.1415;
  //const float fLengthPeriod = 500.0;
  
  const float fHeight = 1.8f;
  const float fPeriod = 8.0*3.1415;
  const float fLengthPeriod = 250.0;
  
  const float fSpeedOfWaving = 1.4f;
  
  float fParameter = mod(fLength, fLengthPeriod);
  float fScale = fParameter/fLengthPeriod;
  
  
  vPositionNoY.y = inPosition.y + sin(-fPeriod/2+fPeriod*fScale+timePassed*fSpeedOfWaving)*fHeight;

	gl_Position = mMVP*vec4(vPositionNoY, 1.0);
  vEyeSpacePos = matrices.viewMatrix*matrices.modelMatrix*vec4(vPositionNoY, 1.0);
  vWorldPos = inPosition;
	vTexCoord = inCoord;
	vNormal = inNormal;

}