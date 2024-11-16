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
uniform vec3 LightPos;
out VS_OUT{
	vec3 FragPos;
	vec3 ViewPos;
	vec3 LightPos;
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
	vs_out.LightPos = TBN * LightPos;
}

#shader fragment
#version 420 core
in VS_OUT{
	vec3 FragPos;
	vec3 ViewPos;
	vec3 LightPos;
	vec2 TexCoord;
}fs_in;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D DepthMap;
uniform float HeightScale;
/*
vec2 ParallaxMapping(vec2 TexCoord, vec3 ViewDir) {
	float Height = texture(DepthMap, TexCoord).r;
	vec2 P = ViewDir.xy / ViewDir.z * Height * HeightScale;
	return TexCoord - P;
}
*/
vec2 ParallaxMapping(vec2 TexCoord, vec3 ViewDir) {
	const float minLayers = 8;
	const float maxLayers = 32;
	float NumLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), ViewDir)));
	float CurrentLayerDepth = 0;
	vec2 CurrentTexCoord = TexCoord;
	float CurrentDepthMapValue = texture(DepthMap, CurrentTexCoord).r;

	while (CurrentDepthMapValue > CurrentLayerDepth) {
		CurrentTexCoord -= ViewDir.xy * HeightScale / NumLayers;
		CurrentLayerDepth += 1 / NumLayers;
		CurrentDepthMapValue = texture(DepthMap, CurrentTexCoord).r;
	}
	vec2 PrevTexCoord = CurrentTexCoord + ViewDir.xy * HeightScale / NumLayers;
	float afterDepth = CurrentDepthMapValue - CurrentLayerDepth;
	float beforeDepth = texture(DepthMap, PrevTexCoord).r - (CurrentLayerDepth - 1 / NumLayers);

	// interpolation of texture coordinates
	float Weight = afterDepth / (afterDepth - beforeDepth);
	vec2 FinalTexCoords = PrevTexCoord * Weight + CurrentTexCoord * (1.0 - Weight);

	return FinalTexCoords;
}
out vec4 FragColor;
void main() {
	//在切线空间计算
	vec3 ViewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
	vec2 TexCoord = ParallaxMapping(fs_in.TexCoord, ViewDir);

	if (TexCoord.x > 1.0 || TexCoord.y > 1.0 || TexCoord.x < 0.0 || TexCoord.y < 0.0)
		discard;
	vec3 LightDir = normalize(fs_in.LightPos - fs_in.FragPos);
	vec3 Normal = texture(NormalMap, TexCoord).rgb;
	Normal = normalize(Normal * 2.0 - 1.0);

	vec3 Color = texture(DiffuseMap, TexCoord).rgb;
	float Diffuse = max(dot(LightDir, Normal), 0);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Specular = pow(max(dot(HalfDir, Normal),0),64);
	vec3 TmpColor = Color * Diffuse + 0.2*Specular;
	TmpColor = pow(TmpColor, vec3(1 / 2.2));
	FragColor = vec4(TmpColor, 1);
}