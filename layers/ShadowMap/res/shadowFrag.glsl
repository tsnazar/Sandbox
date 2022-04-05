#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 v_FragPos;
    vec3 v_Normal;
    vec2 v_TexCoords;
    vec4 v_FragPosLightSpace;
} fs_in;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_DepthMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(u_DepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_DepthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_DepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return shadow;
}

void main()
{
    vec3 color = texture(u_DiffuseTexture, fs_in.v_TexCoords).rgb;
    vec3 normal = normalize(fs_in.v_Normal);
    vec3 lightColor = vec3(1);

    vec3 ambient = 0.3 * lightColor;
    vec3 lightDir = normalize(u_LightPos - fs_in.v_FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(u_ViewPos - fs_in.v_FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    float shadow = ShadowCalculation(fs_in.v_FragPosLightSpace, bias);                      
    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
} 
