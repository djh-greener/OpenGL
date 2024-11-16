#shader vertex
#version 420 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;

void main() {
	gl_Position = projection * view * model * vec4(aPos,1);
	//ÊÀ½ç¿Õ¼ä
	vs_out.FragPos = vec3(model * vec4(aPos, 1));
	vs_out.Normal = mat3(transpose(inverse(model)))* aNormal;
	vs_out.TexCoord = aTexCoord;
}

#shader fragment
#version 420 core
in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;
uniform vec3 viewPos;

struct DirLight {
	vec3 direction;
	vec3 lightColor;
};
uniform DirLight dirLight;

struct Material {
	sampler2D diffuse1;
	float shinness;
};
uniform Material material;

out vec4 FragColor;
vec3 CalcDircLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-dirLight.direction);

	float diffuse = max(dot(lightDir,normal),0);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0), material.shinness);

	return dirLight.lightColor * vec3(texture(material.diffuse1, fs_in.TexCoord)) * (diffuse + specular * 0.2);
}
void main() {
	vec3 normal = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos-fs_in.FragPos);

	vec3 result = CalcDircLight(dirLight, normal, viewDir);

	FragColor = vec4(result, 1);
}