#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vVelocity;
layout (location = 2) in vec3 vGravityVector;
layout (location = 3) in vec3 vColor;
layout (location = 4) in float fLifeTime;
layout (location = 5) in float fSize;
layout (location = 6) in int iType;

out vec3 vPositionPass;
out vec3 vVelocityPass;
out vec3 vGravityPass;
out vec3 vColorPass;
out float fLifeTimePass;
out float fSizePass;
out int iTypePass;

void main()
{
  vPositionPass = vPosition;
  vVelocityPass = vVelocity;
  vGravityPass = vGravityVector;
  vColorPass = vColor;
  fLifeTimePass = fLifeTime;
  fSizePass = fSize;
	iTypePass = iType;
}