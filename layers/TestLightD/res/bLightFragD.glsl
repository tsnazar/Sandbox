#version 330 core
out vec4 v_FragColor;

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

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float Shininess;
};

in DirLight v_DirLight;
in PointLight v_PointLight[NR_POINT_LIGHTS];

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoords;

uniform Material u_Material;

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
  
void main()
{
  vec3 norm = normalize(v_Normal);
  vec3 viewDir = normalize(-v_FragPos);

  vec3 result = CalculateDirectionalLight(v_DirLight, norm, viewDir);

  for(int i = 0; i < NR_POINT_LIGHTS; i++)
  	result += CalculatePointLight(v_PointLight[i], norm, v_FragPos, viewDir);

  v_FragColor = vec4(result, 1.0);
}  

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);

    vec3 ambient = light.Ambient * texture(u_Material.Diffuse, v_TexCoords).rgb;
    vec3 diffuse = light.Diffuse * diff * texture(u_Material.Diffuse, v_TexCoords).rgb;
    vec3 specular = light.Specular * spec * texture(u_Material.Specular, v_TexCoords).rgb;
    
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);

    float dist = length(light.Position - fragPos);
    float attentuation = 1.0f / (light.Constant + light.Linear * dist + light.Quadratic * dist * dist);

    vec3 ambient = light.Ambient * texture(u_Material.Diffuse, v_TexCoords).rgb;
    vec3 diffuse = light.Diffuse * diff * texture(u_Material.Diffuse, v_TexCoords).rgb;
    vec3 specular = light.Specular * spec * texture(u_Material.Specular, v_TexCoords).rgb;

    return attentuation * (ambient + diffuse + specular);
}
