#version 330 core
in vec4 v_FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{             
    vec3 lightToVertex = v_FragPos.xyz - u_LightPos;
    float lightDistance = length(lightToVertex);
    gl_FragDepth = lightDistance / u_FarPlane;
}