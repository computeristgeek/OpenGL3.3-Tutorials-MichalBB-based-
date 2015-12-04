#version 330

#include "blaze_gen_common.geom"

uniform vec3 vGenPosition;

void main()
{
	#include "blaze_gen_update.geom"

  if(iTypeOut == 0)
  {
    EmitVertex();
    EndPrimitive();
    
    if(fTimeLeft < 0.0 && iGenerate == 1)
    { 
      for(int i = 0; i < iNumToGenerate; i++)
      {
        vPositionOut = vGenPosition;
        vVelocityOut = vGenVelocityMin+vec3(vGenVelocityDif.x*rand(), vGenVelocityDif.y*rand(), vGenVelocityDif.z*rand());
  			vGravityOut = vGenGravityVector;
        vColorOut = vGenColor;
        fLifeTimeOut = fLifeMin+fLifeDif*rand();
        fSizeOut = fGenSize;
        iTypeOut = 1;
        EmitVertex();
        EndPrimitive();
      }
    }
  }
  else if(fLifeTimeOut > 0.0)
  {
      EmitVertex();
      EndPrimitive(); 
  }
}