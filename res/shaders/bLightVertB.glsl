#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec3 v_LightPos;
out vec2 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec3 u_LightPos;

void main()
{
	v_FragPos = vec3(u_View * u_Model * vec4(position, 1));
	v_LightPos = vec3(u_View * vec4(u_LightPos, 1));
	v_Normal = mat3(transpose(inverse(u_View * u_Model))) * normal;
	v_TexCoords = texcoords; 
	
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1);
};