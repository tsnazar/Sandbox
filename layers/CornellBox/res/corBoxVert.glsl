#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec3 a_Normal;

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

out vec3 v_Color;
out vec3 v_FragPos;
out vec3 v_Normal;

out PointLight v_Light;
out vec4 v_FragPosLightSpace;

uniform PointLight u_Light; 
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix;

void main()
{
	mat4 modelView = u_View * u_Model;
	v_Color = a_Color;
    v_FragPos = vec3(modelView * vec4(a_Position, 1));
    v_Normal = vec3(transpose(inverse(modelView)) * vec4(a_Normal, 1));
    v_Light = u_Light;
    v_Light.Position = vec3(u_View * vec4(u_Light.Position, 1));
    v_FragPosLightSpace = u_LightSpaceMatrix * u_Model * vec4(a_Position,1);

	gl_Position = u_Projection * modelView * vec4(a_Position, 1);

};