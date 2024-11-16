#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 LightSpaceMatrix;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
}vs_out;
void main() {
	gl_Position = projection * view * model * vec4(aPos, 1);
	vs_out.FragPos = vec3(model * vec4(aPos, 1));
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;
	vs_out.FragPosLightSpace = LightSpaceMatrix * vec4(vs_out.FragPos, 1);
}

#shader fragment
#version 420 core

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
}fs_in;

uniform vec3 LightDir;
uniform sampler2D Diffuse;
uniform sampler2D ShadowMap;
uniform vec3 ViewPos;
out vec4 FragColor;

float	CalcShadow(vec4 FragPosLightSpace)
{
	vec3 ProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	ProjCoords = (ProjCoords + 1) / 2;//从[-1,1]NDC    到    [0,1]
	if (ProjCoords.z > 1.0)
		return 0;
	float ClosestDepth = texture(ShadowMap, ProjCoords.xy).r;
	float CurrentDepth = ProjCoords.z;

	//处理失真，加偏移
	vec3 Normal = normalize(fs_in.Normal);
	vec3 LightDir = normalize(LightDir);
	float bias = max(0.01 * (1.0 - dot(Normal, LightDir)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(ShadowMap, ProjCoords.xy + vec2(x, y) * texelSize).r;
			shadow += CurrentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9;
	return shadow;
};

void main() {
	vec3 LightDir = normalize(LightDir);
	vec3 ViewDir = normalize(ViewPos - fs_in.FragPos);
	vec3 Normal = normalize(fs_in.Normal);
	vec3 SampleColor = texture(Diffuse, fs_in.TexCoord).rgb;
	
	vec3 Ambient = SampleColor * 0.05;
	//diffuse
	vec3 Diffuse = SampleColor * max(dot(LightDir, Normal), 0);
	//specular
	vec3 HalfDir = normalize(LightDir + ViewDir);
	vec3 Specular = SampleColor*pow(max(dot(HalfDir, Normal),0), 32);
	
	
	float Shadow = CalcShadow(fs_in.FragPosLightSpace);
	vec3 TmpColor = Ambient+(1-Shadow)* (Diffuse + Specular);
	//gamma校正
	TmpColor = pow(TmpColor, vec3(1/2.2));
	FragColor = vec4(TmpColor, 1);


}