#version 330 core
out vec4 v_FragColor;

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 v_Normal;
in vec3 v_FragPos;
in vec3 v_LightPos;
in vec2 v_TexCoords;

uniform Light u_Light;
uniform Material u_Material;

void main()
{
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(v_LightPos - v_FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;

    vec3 viewDir = normalize(-v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;

    float dist = length(v_LightPos - v_FragPos);
    float attentuation = 1.0f / (u_Light.constant + u_Light.linear * dist + u_Light.quadratic * dist * dist);

    vec3 result = attentuation * (ambient + diffuse + specular);

	v_FragColor =  vec4(result, 1);
};