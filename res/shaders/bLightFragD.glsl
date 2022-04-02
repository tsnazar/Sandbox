#version 330 core
out vec4 v_FragColor;

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

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
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
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    vec3 ambient = light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;
    
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float dist = length(light.position - fragPos);
    float attentuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;

    return attentuation * (ambient + diffuse + specular);
}
