#shader vertex
#version 420 core
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(Pos,0, 1.0);
};

#shader fragment
#version 420 core
in vec2 TexCoord;
uniform sampler2D GPosition;
uniform sampler2D GNormal;
uniform sampler2D GColorSpec;
uniform vec3 ViewPos;

struct PointLight {
	vec3 Position;
	vec3 Color;
	float Linear;
	float Quadratic;
};
const int NR_LIGHTS = 32;
uniform PointLight Lights[NR_LIGHTS];

out vec4 FragColor;
void main()
{
	// 从G缓冲中获取数据
	vec3 FragPos = texture(GPosition, TexCoord).rgb;
	vec3 Normal = texture(GNormal, TexCoord).rgb;
	vec3 Diffuse = texture(GColorSpec, TexCoord).rgb;
	float Specular = texture(GColorSpec, TexCoord).a;

	
	vec3 lighting = Diffuse * 0.1; // 硬编码环境光照分量
	vec3 ViewDir = normalize(ViewPos - FragPos);
	for (int i = 0; i < NR_LIGHTS; ++i)
	{
		// 漫反射
		vec3 LightDir = normalize(Lights[i].Position - FragPos);

		vec3 diffuse = max(dot(Normal, LightDir), 0.0) * Diffuse * Lights[i].Color;

		vec3 HalfDir = normalize(LightDir + ViewDir);
		float spec = pow(max(dot(Normal, HalfDir), 0.0), 16.0);
		vec3 specular = Lights[i].Color * spec * Specular;

		float distance = length(Lights[i].Position - FragPos);
		float attenuation = 1.0 / (1.0 + Lights[i].Linear * distance + Lights[i].Quadratic * distance * distance);
		lighting += (diffuse+specular)* attenuation;
	}

	FragColor = vec4(lighting, 1.0);
}
