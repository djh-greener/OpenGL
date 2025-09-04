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

	vs_out.TexCoord = aTexCoord;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.ViewPos =ViewPos;
	vs_out.LightDir = LightDir;
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
struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
};
uniform Material material;
uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform float progress;
const float edgeWidth = 0.005;

out vec4 FragColor;
void main()
{
    float maxDistance = length(minVertex - maxVertex);
    float distance = length(fs_in.FragPos - maxVertex);
    float finalDissolve = distance / maxDistance;

	if(progress >= finalDissolve) {
		discard;
	}
	if (progress >= finalDissolve - edgeWidth) {
		float edgeIntensity = (finalDissolve - progress) / edgeWidth;
		vec3 edgeColor = vec3(1.0, 0.5, 0.2);
		FragColor = vec4(edgeColor  * edgeIntensity * 3.0, 1.0);
		return;
	}
	vec3 ViewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
	vec3 LightDir = normalize(-fs_in.LightDir);
	vec3 Normal = normalize(fs_in.Normal);

	vec3 Diffuse = texture(material.diffuse1, fs_in.TexCoord).rgb* max(dot(LightDir, Normal), 0);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	vec3 Specular = texture(material.specular1, fs_in.TexCoord).rgb * pow(max(dot(HalfDir, Normal), 0), 32);
	
	FragColor = vec4(0.05 + Diffuse + Specular, 1.0);
}