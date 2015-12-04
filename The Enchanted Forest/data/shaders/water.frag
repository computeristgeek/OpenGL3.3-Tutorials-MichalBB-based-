#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;

#include "fog.frag"  

uniform sampler2D gSampler;
uniform sampler2D gWaterMap;
uniform vec4 vColor;


out vec4 outputColor;


void main()
{
  vec2 vSpecialCoord = vec2( (vWorldPos.x+600.0f)/1200.0f, 1.0f-(vWorldPos.z+600.0f)/1200.0f);
  vec4 vWaterMap = texture2D(gWaterMap, vSpecialCoord);
  if(vWaterMap.y == 1.0f)discard;
  vec4 vTexColor = texture2D(gSampler, vTexCoord);		
	outputColor = vTexColor*vColor;
  
  if(fogParams.bOn == 1)                                                                                
  {                                                                                                 
	   // Add fog                                                                                     
	   float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);                                          
	   outputColor = mix(outputColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));       
  }  
}