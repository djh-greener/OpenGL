#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 Normal;
out vec3 FragPos;
void main()
{
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 CameraPos;
uniform samplerCube SkyBox;

out vec4 FragColor;
void main()
{
	vec3 I = normalize(FragPos - CameraPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(SkyBox, R).rgb,1);
}