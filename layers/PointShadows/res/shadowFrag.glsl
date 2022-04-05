#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 v_FragPos;
    vec3 v_Normal;
    vec2 v_TexCoords;
} fs_in;

uniform sampler2D u_DiffuseTexture;
uniform samplerCube u_DepthMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

uniform float u_FarPlane;

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - u_LightPos;
    float closestDepth = texture(u_DepthMap, fragToLight).r;
    closestDepth *= u_FarPlane;
    float currentDepth = length(fragToLight);
    float bias = 0.05;
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    
    //FragColor = vec4(vec3(closestDepth / u_FarPlane), 1.0);  
    
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

    float shadow = ShadowCalculation(fs_in.v_FragPos);                      
    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
} 
