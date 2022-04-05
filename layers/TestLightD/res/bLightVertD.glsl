#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

struct DirLight
{
    vec3 Direction;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct PointLight
{
    vec3 Position;

    float Constant;
    float Linear;
    float Quadratic; 

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
#define NR_POINT_LIGHTS 4  

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoords;

out DirLight v_DirLight;
out PointLight v_PointLight[NR_POINT_LIGHTS];

uniform DirLight u_DirLight;
uniform PointLight u_PointLight[NR_POINT_LIGHTS];

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	mat4 modelView = u_View * u_Model;
	v_FragPos = vec3(modelView * vec4(a_Position, 1));
	v_Normal = mat3(transpose(inverse(modelView))) * a_Normal;
	v_TexCoords = a_TexCoords;
	v_DirLight = u_DirLight;
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        v_PointLight[i] = u_PointLight[i];
        v_PointLight[i].Position = vec3(u_View * vec4(u_PointLight[i].Position, 1));
    }

	gl_Position = u_Projection * modelView * vec4(a_Position, 1);
};