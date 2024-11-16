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

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform vec3 viewPos;

uniform samplerCube SkyBox;
out vec4 FragColor;



void main()
{
	//  Ù–‘
	float radio = 1.00 / 1.52;
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 R = refract(-viewDir, norm,radio);

	FragColor = texture(SkyBox, R);
}