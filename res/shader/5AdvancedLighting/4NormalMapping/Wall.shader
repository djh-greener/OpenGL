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
	vec3 Normal;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.FragPos =  vec3(model * vec4(aPos, 1.0));
	vs_out.ViewPos = ViewPos;
	vs_out.LightDir =  LightDir;
	vs_out.TexCoord = aTexCoord;
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
}

#shader fragment
#version 420 core
in VS_OUT{
	vec3 FragPos;
	vec3 ViewPos;
	vec3 LightDir;
	vec2 TexCoord;
	vec3 Normal;
}fs_in;

uniform sampler2D DiffuseMap;

out vec4 FragColor;
void main() {
	vec3 ViewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
	vec3 LightDir = normalize(-fs_in.LightDir);
	vec3 Normal = normalize(fs_in.Normal);

	vec3 Color = texture(DiffuseMap, fs_in.TexCoord).rgb;
	float Diffuse = max(dot(LightDir, Normal), 0);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Specular = pow(max(dot(HalfDir, Normal),0),32);
	vec3 TmpColor = Color * Diffuse + 0.2 * Specular;
	TmpColor = pow(TmpColor, vec3(1 / 2.2));
	FragColor = vec4(TmpColor, 1);
}