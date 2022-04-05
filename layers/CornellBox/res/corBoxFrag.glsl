#version 330 core
out vec4 v_FragColor;

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

in vec3 v_Color;
in vec3 v_FragPos;
in vec3 v_Normal;
in PointLight v_Light;
in vec4 v_FragPosLightSpace;

uniform bool u_Blinn;
uniform sampler2D u_DepthMap;
 
 float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(u_DepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;;

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
    vec3 lightDir = normalize(v_Light.Position - v_FragPos);
    vec3 normal = normalize(v_Normal);

    float diff = max(dot(normal, lightDir), 0.0f);

    float spec = 0.0f;

    if(diff > 0)
    {
        if(u_Blinn)
        {
            vec3 halfwayDir = normalize(lightDir + normalize(-v_FragPos));
            spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
        }
        else
        {
            vec3 reflectDir = reflect(-lightDir, normal);
            spec = pow(max(dot(normalize(-v_FragPos), reflectDir), 0.0f), 2.0f);
        }   
    }

    float dist = length(v_Light.Position - v_FragPos);
    float attentuation = 1.0f / (v_Light.Constant + v_Light.Linear * dist + v_Light.Quadratic * dist * dist);
  
    vec3 ambient = v_Light.Ambient;
    vec3 diffuse = v_Light.Diffuse * diff;
    vec3 specular = v_Light.Specular * spec;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    float shadow = ShadowCalculation(v_FragPosLightSpace, bias);
    vec3 result = ambient + (1 - shadow) * (diffuse + specular);

    v_FragColor = vec4(attentuation * result * v_Color, 1.0);
}  

