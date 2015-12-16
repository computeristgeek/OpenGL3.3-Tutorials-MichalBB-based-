#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 9)out;

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;                                                                           
	mat4 normalMatrix;
} matrices;

in vec2 vTexCoordPass[];
in vec3 vNormalPass[];

smooth out vec3 vEyeSpacePos;
smooth out vec3 vNormal;
smooth out vec3 vWorldPos;
smooth out vec2 vTexCoord;

uniform float fBender;

void main()
{
    mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;
    vec3 vMiddle = (gl_in[0].gl_Position.xyz+gl_in[1].gl_Position.xyz+gl_in[2].gl_Position.xyz)/3.0+(vNormalPass[0]+vNormalPass[1]+vNormalPass[2])*fBender;
    vec2 vTexCoordMiddle = (vTexCoordPass[0]+vTexCoordPass[1]+vTexCoordPass[2])/3.0;
    vec3 vNormalPass2[3];
    for(int i = 0; i < 3; i++)vNormalPass2[i] = (vec4(vNormalPass[i], 1.0)*matrices.normalMatrix).xyz;
    vec3 vNormalMiddle = (vNormalPass2[0]+vNormalPass2[1]+vNormalPass2[2])/3.0;
    for(int i = 0; i < 3; i++)
    {
      vec3 vPos = gl_in[i].gl_Position.xyz;
      gl_Position = mMVP*vec4(vPos, 1.0);
      vNormal = (vec4(vNormalPass2[i], 1.0)).xyz;
      vTexCoord = vTexCoordPass[i];
      EmitVertex();

      vPos = gl_in[(i+1)%3].gl_Position.xyz;
      gl_Position = mMVP*vec4(vPos, 1.0);
      vNormal = (vec4(vNormalPass2[(i+1)%3], 1.0)).xyz;
      vTexCoord = vTexCoordPass[(i+1)%3];
      EmitVertex();

      gl_Position = mMVP*vec4(vMiddle, 1.0);
      vNormal = vNormalMiddle;
      vTexCoord = vTexCoordMiddle;
      EmitVertex();

      EndPrimitive();
    }
}
