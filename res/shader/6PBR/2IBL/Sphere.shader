#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}vs_out;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = mat3(transpose(inverse(model)))*aNormal;
	vs_out.TexCoords = aTexCoords;

};

#shader fragment
#version 330 core

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform vec3 ViewPos;
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform samplerCube irradianceMap;


struct PointLight {
	vec3 Position;
	vec3 Color;
};
#define NR_POINT_LIGHTS 4
#define PI 3.1415926
uniform PointLight Lights[NR_POINT_LIGHTS];
out vec4 FragColor;

vec3 Fresnel(float theta, vec3 F0)
{
	return F0 + (1 - F0) * pow(clamp(1 - cos(theta), 0, 1), 5);
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denominator = NdotH2 * (a2 - 1.0) + 1.0;
	denominator = PI * denominator * denominator;

	return  a2 / denominator;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

void main()
{
	vec3 N = normalize(fs_in.Normal);
	vec3 V = normalize(ViewPos - fs_in.FragPos);
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	vec3 Lo = vec3(0);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		vec3 L = normalize(Lights[i].Position - fs_in.FragPos);
		vec3 H = normalize(L + V);
		
		float Distance = length(Lights[i].Position - fs_in.FragPos);
		float attenutation = 1 / (Distance * Distance);
		vec3 Radiance = Lights[i].Color * attenutation;


		vec3 F = Fresnel(max(dot(H, V),0), F0);
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);

		vec3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator/ denominator;

		vec3 Ks = F;
		vec3 Kd = vec3(1) - Ks;
		Kd *= 1 - metallic;//金属没有漫反射

		float NdotL = max(dot(N, L), 0);
		Lo += (specular + Kd * albedo / PI)*Radiance * NdotL ;
	}
	vec3 Ks = Fresnel(max(dot(N, V), 0), F0);
	vec3 Kd = vec3(1) - Ks;
	Kd *= 1 - metallic;
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;
	vec3 ambient = diffuse * Kd * ao;

	vec3 color = ambient+Lo;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));
	FragColor = vec4(color,1);
}
