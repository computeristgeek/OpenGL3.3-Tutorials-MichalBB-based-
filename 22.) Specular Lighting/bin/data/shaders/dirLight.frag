#version 330

#include_part

struct DirectionalLight
{
	vec3 vColor;
	vec3 vDirection;

	float fAmbient;
   int iSkybox;
};

vec4 GetDirectionalLightColor(DirectionalLight dirLight, vec3 vNormal);

struct Material
{
   float fSpecularIntensity;
   float fSpecularPower;
};

vec4 GetSpecularColor(vec3 vWorldPos, vec3 vEyePos, vec3 vNormal, Material mat, DirectionalLight dLight);

#definition_part

vec4 GetDirectionalLightColor(DirectionalLight dirLight, vec3 vNormal)
{
	float fDiffuseIntensity = max(0.0, dot(vNormal, -dirLight.vDirection));
	float fMult = clamp(dirLight.fAmbient+fDiffuseIntensity, 0.0, 1.0);
   if(dirLight.iSkybox == 1)fMult = dirLight.fAmbient;
	return vec4(dirLight.vColor*fMult, 1.0);
}

vec4 GetSpecularColor(vec3 vWorldPos, vec3 vEyePos, vec3 vNormal, Material mat, DirectionalLight dLight)
{
   vec4 vResult = vec4(0.0, 0.0, 0.0, 0.0);
   
   vec3 vReflectedVector = normalize(reflect(dLight.vDirection, vNormal));
   vec3 vVertexToEyeVector = normalize(vEyePos-vWorldPos);
   float fSpecularFactor = dot(vVertexToEyeVector, vReflectedVector);
   
   if (fSpecularFactor > 0)
   {
      fSpecularFactor = pow(fSpecularFactor, mat.fSpecularPower);
      vResult = vec4(dLight.vColor, 1.0) * mat.fSpecularIntensity * fSpecularFactor;  
   }
   return vResult;
}