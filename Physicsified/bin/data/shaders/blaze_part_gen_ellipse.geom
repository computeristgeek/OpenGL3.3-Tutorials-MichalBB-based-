#version 330

#include "blaze_gen_common.geom"

uniform vec3 vGenPosition;
uniform float fRadiusX, fRadiusY, fRadiusZ;

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
				float fAngle = 2.0*3.1415*rand();
				vec3 vVectorA = vec3(cos(fAngle), 0.0, sin(fAngle));
				vec3 vVectorB = vec3(0.0, 1.0, 0.0);
	
				fAngle = 2.0*3.1415*rand();
				float fSine = sin(fAngle);
				float fCosine = cos(fAngle);

				float fRandRadius = fRadiusX*rand();
        
        vPositionOut = vGenPosition+(vVectorA*fCosine + vVectorB*fSine)*fRandRadius;
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