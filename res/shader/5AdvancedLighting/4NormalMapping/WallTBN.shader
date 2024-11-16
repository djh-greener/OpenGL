#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 ViewPos;
uniform vec3 LightDir;
out VS_OUT{
	vec3 FragPos;
	vec3 ViewPos;
	vec3 LightDir;
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.TexCoord = aTexCoord;
	mat3 NormalMat = transpose(inverse(mat3(model)));
	vec3 T = normalize(NormalMat * aTangent);
	vec3 B = normalize(NormalMat * aBiTangent);
	vec3 N = normalize(NormalMat * aNormal);
	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.FragPos = TBN * vec3(model * vec4(aPos, 1.0));
	vs_out.ViewPos = TBN * ViewPos;
	vs_out.LightDir = TBN * LightDir;
}

#shader fragment
#version 420 core
in VS_OUT{
	vec3 FragPos;
	vec3 ViewPos;
	vec3 LightDir;
	vec2 TexCoord;
}fs_in;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

out vec4 FragColor;
void main() {
	//在切线空间计算
	vec3 ViewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
	vec3 LightDir = normalize(-fs_in.LightDir);
	vec3 Normal = texture(NormalMap, fs_in.TexCoord).rgb;
	Normal = normalize(Normal * 2.0 - 1.0);

	vec3 Color = texture(DiffuseMap, fs_in.TexCoord).rgb;
	float Diffuse = max(dot(LightDir, Normal), 0);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Specular = pow(max(dot(HalfDir, Normal),0),64);
	vec3 TmpColor = Color * Diffuse + 0.2*Specular;
	TmpColor = pow(TmpColor, vec3(1 / 2.2));
	FragColor = vec4(TmpColor, 1);
}