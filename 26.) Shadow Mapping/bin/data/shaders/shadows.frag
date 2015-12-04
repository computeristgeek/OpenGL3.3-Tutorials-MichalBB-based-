#version 330

#include_part

float GetVisibility(sampler2D shadowMap, vec4 vShadowCoord);

#definition_part

uniform int bShadowsOn;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float GetVisibility(sampler2D shadowMap, vec4 vShadowCoord)
{
    if(bShadowsOn == 0)return 1.0;
    float visibility = 1.0;
    float bias = 0.005;
 
    vec4 shadowCoordinateWdivide = vec4(vShadowCoord.x/vShadowCoord.w, vShadowCoord.y/vShadowCoord.w, (vShadowCoord.z-bias)/vShadowCoord.w, 1.0) ;

    for (int i=0;i<4;i++)
    {
      int index = i;
		  vec4 vShadowSmooth = vec4(vShadowCoord.x + poissonDisk[index].x/700.0, vShadowCoord.y + poissonDisk[index].y/700.0, (vShadowCoord.z-bias)/vShadowCoord.w, 1.0);
		  float fSub = texture(shadowMap, vShadowSmooth.st).r; 
		  visibility -= 0.1*(1.0-fSub);
    }
    return visibility;
}