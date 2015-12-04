#version 330

#include_part

vLocalSeed = vSeed;

vPositionOut = vPositionPass[0];
vVelocityOut = vVelocityPass[0];
if(iTypePass[0] != 0)vPositionOut += vVelocityOut*fDeltaTime;
 
if(iTypePass[0] != 0)vVelocityOut += vGravityPass[0]*fDeltaTime;
vGravityOut = vGravityPass[0];
vColorOut = vColorPass[0];
fLifeTimeOut = fLifeTimePass[0]-fDeltaTime;
fSizeOut = fSizePass[0];
iTypeOut = iTypePass[0];

gl_Position = vec4(1.0, 1.0, 1.0, 1.0);