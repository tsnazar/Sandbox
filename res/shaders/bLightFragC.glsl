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

    float cutOff;
    float outerCutOff;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoords;

uniform Light u_Light;
uniform Material u_Material;

void main()
{
    
    vec3 lightDir = normalize(-v_FragPos);

    float theta = dot(lightDir, vec3(0.0f, 0.0f, 1.0f));
    float epsilon = u_Light.cutOff - u_Light.outerCutOff;
    float intensity = clamp((theta - u_Light.outerCutOff) / epsilon, 0.0, 1.0);
    //float intensity = smoothstep(0.0, 1.0, (theta - u_Light.outerCutOff) / epsilon);

    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;

    vec3 norm = normalize(v_Normal);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;

    vec3 viewDir = normalize(-v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;

    float dist = length(-v_FragPos);
    float attentuation = 1.0f / (u_Light.constant + u_Light.linear * dist + u_Light.quadratic * dist * dist);

    vec3 result =  intensity * attentuation * (ambient + diffuse + specular);
        
    v_FragColor =  vec4(result, 1);
};