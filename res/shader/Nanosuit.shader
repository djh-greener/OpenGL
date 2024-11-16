#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	//vec3 Normal;
	//vec3 FragPos;
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//vs_out.Normal = vec3(transpose(inverse(view * model)) * vec4(aNormal, 1));
	//vs_out.FragPos = vec3(view * model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;
};

#shader fragment
#version 420 core
in VS_OUT{
	//vec3 Normal;
	//vec3 FragPos;
	vec2 TexCoord;
}fs_in;
struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
};
uniform Material material;
out vec4 FragColor;
void main()
{
	vec3 result = vec3(texture(material.diffuse1, fs_in.TexCoord));
	FragColor = vec4(result, 1.0);
}