#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = vec3(transpose(inverse(model))  * vec4(aNormal,1));
    TexCoord = aTexCoord;
};

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform vec3 viewPos;
struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    float shininess;
};
uniform Material material;
out vec4 FragColor;


struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, TexCoord));
	vec3 specular1 = vec3(texture(material.specular1, TexCoord));
	if (specular1.x == 0.0)
		specular1 = vec3(texture(material.diffuse1, TexCoord));
	
	vec3	specular = light.specular * spec * specular1;

	
    return (ambient + diffuse + specular);
}

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, TexCoord));
	vec3 specular1 = vec3(texture(material.specular1, TexCoord));
	if (specular1.x == 0.0)
		specular1 = light.specular * spec * vec3(texture(material.diffuse1, TexCoord));
	vec3 specular = light.specular * spec * specular1;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

struct SpotLight {
    vec3 position;
    vec3  direction;
    float cutOff; 
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = vec3(texture(material.diffuse1, TexCoord)) * light.ambient;

    vec3 diffuse = intensity * light.diffuse * vec3(texture(material.diffuse1, TexCoord)) * max(dot(normal, lightDir), 0);

    vec3 h = normalize(lightDir + viewDir);
	vec3 specular1 = vec3(texture(material.specular1, TexCoord));
	if (specular1.x == 0.0)
		specular1 = vec3(texture(material.diffuse1, TexCoord));
	vec3 specular = intensity * light.specular * specular1* pow(max(dot(h, normal), 0), material.shininess);
    return attenuation * (ambient + diffuse + specular);
}

void main()
{
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

    FragColor = vec4(result, 1.0);
	//FragColor = texture(material.diffuse1, TexCoord);
}