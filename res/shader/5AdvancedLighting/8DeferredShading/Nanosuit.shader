#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.Normal = vec3(transpose(inverse(model)) * vec4(aNormal, 1));
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;
};

#shader fragment
#version 420 core
in VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}fs_in;
struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
};
uniform Material material;
layout(location = 0)out vec3 GPosition;
layout(location = 1)out vec3 GNormal;
layout(location = 2)out vec4 GColorSpec;

void main()
{
	GPosition = fs_in.FragPos;

	GNormal = normalize(fs_in.Normal);

	GColorSpec.rgb = texture(material.diffuse1, fs_in.TexCoord).rgb;
	GColorSpec.a = texture(material.specular1, fs_in.TexCoord).r;

}