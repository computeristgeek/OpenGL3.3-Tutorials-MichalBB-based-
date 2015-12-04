#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec4 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;

uniform sampler2D gSampler;
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

smooth in vec4 ShadowCoord;

void main()
{
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor = texture2D(gSampler, vTexCoord);
  if(vTexColor.x == 0.0 && vTexColor.y == 0.0 && vTexColor.z == 0)discard;
	vec4 vMixedColor = vTexColor*vColor;
  vec4 vDirLightColor = getDirectionalLightColor(sunLight, vNormalized);
  vec4 vSpotlightColor = GetSpotLightColor(spotLight, vWorldPos);
  vec4 vPointLightColor = getPointLightColor(ptLight[0], vWorldPos, vNormalized)+getPointLightColor(ptLight[1], vWorldPos, vNormalized)+getPointLightColor(ptLight[2], vWorldPos, vNormalized);
		
  float visibility  = GetVisibility(shadowMap, ShadowCoord);
    
	outputColor = vMixedColor*(vDirLightColor+vSpotlightColor+vPointLightColor)*visibility;
  if(fogParams.bOn == 1)
  {
	   // Add fog            
	   float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);                               
	   outputColor = mix(outputColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));
  }
}