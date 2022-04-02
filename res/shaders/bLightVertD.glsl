#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
	v_FragPos = vec3(modelView * vec4(position, 1));
	v_Normal = mat3(transpose(inverse(modelView))) * normal;
	v_TexCoords = texcoords;
	v_DirLight = u_DirLight;
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        v_PointLight[i] = u_PointLight[i];
        v_PointLight[i].position = vec3(u_View * vec4(u_PointLight[i].position, 1));
    }

	gl_Position = u_Projection * modelView * vec4(position, 1);
};