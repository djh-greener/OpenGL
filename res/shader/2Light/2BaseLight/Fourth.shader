#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int exponent;

out vec3 LightingColor;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = vec3(transpose(inverse(model)) * vec4(aNormal, 1));
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = lightColor * diffuseStrength * max(dot(normal, lightDir), 0);

    vec3 eyeDir = normalize(viewPos - FragPos);
    vec3 h = normalize(lightDir + eyeDir);
    vec3 specular = lightColor * specularStrength * pow(max(dot(h, normal), 0), exponent);

    vec3 result = ambient + diffuse + specular;
    LightingColor = result;
};

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 LightingColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(LightingColor * objectColor, 1.0);
}
