#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;

smooth in vec4 vEyeSpacePos;

in vec3 LightDirection_tangentspace;

uniform sampler2D gSampler[2];
uniform sampler2D shadowMap;
uniform vec4 vColor;

#include "dirLight.frag"
#include "spotLight.frag"
#include "pointLight.frag"

#include "fog.frag"

#include "shadow.frag"     
                        
uniform DirectionalLight sunLight;
uniform SpotLight spotLight;
uniform PointLight ptLight[3];

out vec4 outputColor;

smooth in vec4 ShadowCoord;

void main()
{

	vec4 vTexColor = texture2D(gSampler[0], vTexCoord);
	vec3 vNormalExtr = normalize(texture2D(gSampler[1], vTexCoord).rgb*2.0 - 1.0);

	vec4 vMixedColor = vTexColor*vColor;
  
  vec3 vLD_tangentspaceNorm = normalize(LightDirection_tangentspace);
  
  	float fDiffuseIntensity = max(0.0, dot(vNormalExtr, -vLD_tangentspaceNorm));
   float fMult = clamp(sunLight.fAmbient+fDiffuseIntensity, 0.0, 1.0);
   vec4 vDirLightColor =  vec4(sunLight.vColor*fMult, 1.0);
  
	vec4 vSpotlightColor = GetSpotLightColor(spotLight, vWorldPos);
   vec4 vPointLightColor = getPointLightColor(ptLight[0], vWorldPos, vNormalExtr)+getPointLightColor(ptLight[1], vWorldPos, vNormalExtr)+getPointLightColor(ptLight[2], vWorldPos, vNormalExtr);
		
    
	float visibility = GetVisibility(shadowMap, ShadowCoord);
    
	outputColor = vMixedColor*(vDirLightColor+vSpotlightColor+vPointLightColor)*visibility;
  
  if(fogParams.bOn == 1)                                                                                
  {                                                                                                 
	   // Add fog                                                                                     
	   float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);                                          
	   outputColor = mix(outputColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));       
  }                                                                                                 
}