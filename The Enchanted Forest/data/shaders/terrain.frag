#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;

uniform sampler2D gSampler[5];
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
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor = vec4(0.0);
	
	float fScale = vWorldPos.y/125.0f;

	const float fRange1 = 0.15f;
	const float fRange2 = 0.3f;
	const float fRange3 = 0.65f;
	const float fRange4 = 0.85f;

	if(fScale >= 0.0 && fScale <= fRange1)vTexColor = texture2D(gSampler[0], vTexCoord);
	else if(fScale <= fRange2)
	{
		fScale -= fRange1;
		fScale /= (fRange2-fRange1);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(gSampler[0], vTexCoord)*fScale;
		vTexColor += texture2D(gSampler[1], vTexCoord)*fScale2;
	}
	else if(fScale <= fRange3)vTexColor = texture2D(gSampler[1], vTexCoord);
	else if(fScale <= fRange4)
	{
		fScale -= fRange3;
		fScale /= (fRange4-fRange3);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(gSampler[1], vTexCoord)*fScale;
		vTexColor += texture2D(gSampler[2], vTexCoord)*fScale2;		
	}
	else vTexColor = texture2D(gSampler[2], vTexCoord);

	vec2 vPathCoord = vTexCoord*0.02;
	vPathCoord.y = 1-vPathCoord.y;
	
	vec4 vPathInt = texture2D(gSampler[4], vPathCoord);
	fScale = vPathInt.x;
	vec4 vPathColor = texture2D(gSampler[3], vTexCoord);
	vec4 vFinalTexColor = fScale*vTexColor+(1-fScale)*vPathColor;

	vec4 vMixedColor = vFinalTexColor*vColor;
	vec4 vDirLightColor = getDirectionalLightColor(sunLight, vNormal);
	vec4 vSpotlightColor = GetSpotLightColor(spotLight, vWorldPos);
  vec4 vPointLightColor = getPointLightColor(ptLight[0], vWorldPos, vNormalized)+getPointLightColor(ptLight[1], vWorldPos, vNormalized)+getPointLightColor(ptLight[2], vWorldPos, vNormalized);

  float visibility = GetVisibility(shadowMap, ShadowCoord);

  //if(gl_FragCoord.z > visibility)visibility = 0.5;
	//if(bShadowsON == 0)visibility = 1.0;	
	outputColor = vMixedColor*(vDirLightColor+vSpotlightColor+vPointLightColor)*visibility;
  
  if(fogParams.bOn == 1)                                                                                
  {                                                                                                 
	   // Add fog                                                                                     
	   float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);                                          
	   outputColor = mix(outputColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));       
  } 
}