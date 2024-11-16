#shader vertex
#version 420 core
layout(location = 0)in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;

out vec3 LocalPos;
void main() {
	LocalPos = aPos;
	vec4 Pos = projection * mat4(mat3(view)) * vec4(aPos, 1);
	gl_Position = Pos.xyww;
}

#shader fragment
#version 420 core
in vec3 LocalPos;

const float PI = 3.14159265359;
float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}
//围绕N进行重要性采样
vec3 ImportanceSampleGGX(vec2 X, vec3 N, float roughness)
{
	float a = roughness * roughness;

	float phi = 2 * PI * X.x;
	float cosTheta = sqrt((1 - X.y) / (1 + (a * a - 1) * X.y));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);

	vec3 up = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);//为什么？
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVec = tangent * cos(phi) * sinTheta + bitangent * sin(phi) * sinTheta + N * cosTheta;
	return normalize(sampleVec);
}
float DistributionGGX(float NdotH, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH2 = NdotH * NdotH;

	float denominator = NdotH2 * (a2 - 1.0) + 1.0;
	denominator = PI * denominator * denominator;

	return  a2 / denominator;
}

uniform samplerCube EnvMap;
uniform float roughness;
out vec4 FragColor;

void main() {
	vec3 N = normalize(LocalPos);
	vec3 V = N;//观察方向=法线方向=采样中心
	vec3 prefilterColor = vec3(0.0);


	const uint SAMPLE_COUNT = 1024u;
	float totalWeight = 0;
	for (uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L = normalize(2 * H * dot(H, V) - V);//H=norm(L+V)
		float NdotL = max(dot(N, L), 0);
		if (NdotL > 0)
		{
			float D = DistributionGGX(NdotL, roughness);
			float pdf = D*dot(N,H) / (4*dot(H,V)) + 0.0001;
			float resolution = 512;
			float saTexel = 4 * PI / (6 *resolution * resolution);//每个立方体面的像素的立体角
			float saSample = 1 / (float(SAMPLE_COUNT )* pdf + 0.001);
			float MipLevel = roughness == 0 ? 0 : 0.5 * log2(saSample / saTexel); //saSample越大，采样越不足，需要在更模糊的Mipmap上采样环境贴图

			prefilterColor += textureLod(EnvMap, L, MipLevel).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	prefilterColor = prefilterColor / totalWeight;
	FragColor = vec4(prefilterColor, 1);
}