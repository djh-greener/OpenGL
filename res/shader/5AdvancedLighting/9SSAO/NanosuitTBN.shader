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

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoord;
	mat3 TBN;
	vec3 N;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.FragPos = vec3(view * model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;

	mat3 NormalMat = transpose(inverse(mat3(view * model)));
	vec3 T = normalize(NormalMat * aTangent);
	vec3 B = normalize(NormalMat * aBiTangent);
	vec3 N = normalize(NormalMat * aNormal);
	vs_out.TBN = mat3(T, B, N);
	vs_out.N = N;
};

#shader fragment
#version 420 core
in VS_OUT{
	vec3 FragPos;
	vec2 TexCoord;
	mat3 TBN;
	vec3 N;
}fs_in;
struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
	sampler2D normal1;
};
uniform Material material;
layout(location = 0)out vec3 GPosition;
layout(location = 1)out vec3 GNormal;
layout(location = 2)out vec4 GColorSpec;


const float NEAR = 0.1; // 投影矩阵的近平面
const float FAR = 50.0f; // 投影矩阵的远平面


void main()
{
	GPosition.rgb = fs_in.FragPos;

	GNormal = texture(material.normal1, fs_in.TexCoord).rgb;
	GNormal = normalize(GNormal * 2 - 1);
	GNormal = normalize(fs_in.TBN * GNormal);
	//GColorSpec.rgb = texture(material.diffuse1, fs_in.TexCoord).rgb;
	GColorSpec.rgb = vec3(0.5);
	GColorSpec.a = texture(material.specular1, fs_in.TexCoord).r;

}