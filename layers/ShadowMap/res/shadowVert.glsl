#version 330 core
layout (location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT {
    vec3 v_FragPos;
    vec3 v_Normal;
    vec2 v_TexCoords;
    vec4 v_FragPosLightSpace;
} vs_out;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_LightSpaceMatrix;

void main()
{
    vs_out.v_FragPos = vec3(u_Model * vec4(a_Position, 1));
    vs_out.v_Normal = transpose(inverse(mat3(u_Model))) * a_Normal;
    vs_out.v_TexCoords = a_TexCoords;
    vs_out.v_FragPosLightSpace = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1);
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1);
}