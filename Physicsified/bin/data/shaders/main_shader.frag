#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec4 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

#include "dirLight.frag"
#include "pointLight.frag"
#include "spotLight.frag"

uniform DirectionalLight sunLight;
uniform PointLight ptLight;
uniform PointLight ptFire;
uniform SpotLight slFlashLight;
uniform int ignoreLights;

void main()
{
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor = texture2D(gSampler, vTexCoord);

	vec4 vMixedColor = vTexColor*vColor;
  vec4 vDirLightColor = GetDirectionalLightColor(sunLight, vNormalized);
  vec4 vPointLightColor = GetPointLightColor(ptLight, vWorldPos, vNormalized);
  vec4 vFireColor = GetPointLightColor(ptFire, vWorldPos, vNormalized);
  vec4 vFlashLightColor = GetSpotLightColor(slFlashLight, vWorldPos);

  vec4 vMult = vec4(0.2, 0.2, 0.2, 0.2);
  if(ignoreLights == 0)vMult = vDirLightColor+vPointLightColor+vFlashLightColor+vFireColor;
	outputColor = vMixedColor*(vMult);
}