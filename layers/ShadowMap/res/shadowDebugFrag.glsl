#version 330 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D u_DepthMap;
uniform float u_FarPlane;
uniform float u_NearPlane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}

void main()
{
    FragColor = vec4(vec3(LinearizeDepth(texture(u_DepthMap, v_TexCoords).r) / u_FarPlane),1.0);
    //FragColor = vec4(vec3(texture(u_DepthMap, v_TexCoords).r),1.0);
} 