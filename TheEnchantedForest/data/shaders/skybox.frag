#version 330

#include "fog.frag" 

smooth in vec2 vTexCoord;
smooth in vec4 vEyeSpacePos;
uniform sampler2D gSampler;

uniform float fAmbient;

out vec4 outputColor;

void main()
{
  vec4 vTexColor = texture2D(gSampler, vTexCoord);		
	outputColor = vTexColor*fAmbient;
  
  if(fogParams.bOn == 1)                                                                                
  {                                                                                                 
	   // Add fog                                                                                     
	   float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);                                          
	   outputColor = mix(outputColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));       
  } 
}