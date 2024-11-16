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
uniform sampler2D Occlusion;
uniform vec3 ViewPos;

struct DirectionalLight {
	vec3 Direction;
	vec3 Color;
};

uniform DirectionalLight Light;
uniform int Method;

out vec4 FragColor;
void main()
{
	 //从G缓冲中获取数据
	vec3 FragPos = texture(GPosition, TexCoord).rgb;
	vec3 Normal = texture(GNormal, TexCoord).rgb;
	vec3 Diffuse = texture(GColorSpec, TexCoord).rgb;
	float Specular = texture(GColorSpec, TexCoord).a;
	vec3 ambient = 1* Diffuse*(texture(Occlusion, TexCoord).r);
	
	vec3 ViewDir = normalize( - FragPos);
	
	
	// 漫反射
	vec3 LightDir = normalize(-Light.Direction);
	
	vec3 diffuse = max(dot(Normal, LightDir), 0.0) * Diffuse * Light.Color;
	
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float spec = pow(max(dot(Normal, HalfDir), 0.0), 16.0);
	vec3 specular = Light.Color * spec * Specular;

	vec3 lighting = (ambient + 0.3*(diffuse + specular));
	if (Method == 0)
		FragColor = vec4(lighting, 1.0);
	if (Method == 1)
		FragColor = vec4(FragPos, 1.0);
	if (Method == 2)
		FragColor = vec4(Normal, 1.0);
	if (Method == 3)
		FragColor = vec4(Diffuse, 1.0);
	if (Method == 4)
		FragColor = vec4(texture(Occlusion, TexCoord).r,0.0,0.0,1.0);
}
