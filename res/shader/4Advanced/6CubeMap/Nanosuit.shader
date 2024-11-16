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
struct Material {
	sampler2D ambient1;
	sampler2D diffuse1;
	//sampler2D specular1;
};
uniform Material material;

uniform samplerCube SkyBox;
out vec4 FragColor;


vec3 CalcReflect(vec3 normal, vec3 viewDir)
{
	vec3 diffuse = vec3(texture(material.diffuse1, TexCoord));
	// 从天空盒采样镜面光
	vec3 R = reflect(-viewDir, normal);
	vec3 spec = vec3(texture(SkyBox, R));
	//材质本身的镜面反射系数
	vec3 specular = vec3(texture(material.ambient1, TexCoord));

	return diffuse + spec * specular;
}

void main()
{
	// 属性
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcReflect(norm, viewDir);

	FragColor = vec4(result, 1.0);
}