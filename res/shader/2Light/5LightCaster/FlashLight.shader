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
    Normal = vec3(transpose(inverse(model)) * vec4(aNormal, 1));
    TexCoord = aTexCoord;
};

#shader fragment
#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3  direction;
    float cutOff; //”‡œ“
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 viewPos;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient;

    vec3 normal = normalize(Normal);

    vec3 diffuse = intensity*light.diffuse * vec3(texture(material.diffuse, TexCoord)) * max(dot(normal, lightDir), 0);

    vec3 eyeDir = normalize(viewPos - FragPos);
    vec3 h = normalize(lightDir + eyeDir);
    vec3 specular = intensity*light.specular * vec3(texture(material.specular, TexCoord)) * pow(max(dot(h, normal), 0), material.shininess);

    vec3 result = attenuation * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);

}