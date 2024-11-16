#shader vertex
#version 420 core
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(Pos, 0, 1.0);
};

#shader fragment
#version 420 core
in vec2 TexCoord;

uniform sampler2D GPosition;
uniform sampler2D GNormal;
uniform sampler2D TexNoise;
uniform mat4 projection;
uniform vec3 samples[64];
uniform int SCR_WIDTH;
uniform int SCR_HEIGHT;

const vec2 noiseScale = vec2(SCR_WIDTH / 4.0, SCR_HEIGHT / 4.0);
const int KernelSize = 64;
const float Radius = 2;

out float  FragColor;
void main()
{
	vec3 FragPos = texture(GPosition, TexCoord).rgb;
	vec3 normal = normalize(texture(GNormal, TexCoord).rgb);
	vec3 randomVec = normalize(texture(TexNoise, TexCoord * noiseScale).rgb);
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;

	for (int i = 0; i < KernelSize; i++) {
		
		vec3 sampleView = FragPos + TBN * samples[i]* Radius;
		//为了得到采样点的深度和纹理坐标，需要从观察空间到屏幕空间 推理一遍
		vec4 offset = vec4(sampleView, 1);
		offset = projection * offset; 
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;
		float sampleDepth = -texture(GPosition, offset.xy).z;
		
		float rangeCheck = smoothstep(0.0, 1.0, Radius / abs(-FragPos.z - sampleDepth));
		occlusion += (sampleDepth < -sampleView.z ? 1.0 : 0.0)*rangeCheck;
	}
	FragColor = 1.0 - (occlusion / KernelSize);
}
