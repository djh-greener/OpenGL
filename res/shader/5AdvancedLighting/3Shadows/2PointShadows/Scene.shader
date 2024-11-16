#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool ReverseNormal;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;
void main() {
	gl_Position = projection * view * model * vec4(aPos, 1);
	vs_out.FragPos = vec3(model * vec4(aPos, 1));
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	if(ReverseNormal)
		vs_out.Normal *=-1;

	vs_out.TexCoord = aTexCoord;
}

#shader fragment
#version 420 core

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;

uniform sampler2D Diffuse;
uniform samplerCube DepthMap;

uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform float FarPlane;

out vec4 FragColor;

float	CalcShadow(vec3 FragPos)
{
	vec3 Direc = FragPos - LightPos;
	float CurrentDepth = length(Direc);

	float bias = 0.05;
	float shadow = 0.0;
	float viewDistance = length(ViewPos - FragPos);
	float diskRadius = (1.0 + (viewDistance / FarPlane)) / 25.0;
	vec3 sampleOffsetDirections[20] = vec3[]
	(
		vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
		vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
		vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
		vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
		vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
		);
	for (int i=0;i<20;i++)
	{

		float ClosestDepth = texture(DepthMap, Direc + diskRadius*sampleOffsetDirections[i]).r;
		shadow += CurrentDepth - bias > ClosestDepth *FarPlane ? 1.0 : 0.0;
			
	}
	shadow /= 20;
	return shadow;
};

void main() {
	vec3 LightDir = normalize(LightPos- fs_in.FragPos);
	vec3 ViewDir = normalize(ViewPos - fs_in.FragPos);
	vec3 Normal = normalize(fs_in.Normal);
	vec3 SampleColor = texture(Diffuse, fs_in.TexCoord).rgb;

	vec3 Ambient = SampleColor * 0.05;
	//diffuse
	vec3 Diffuse = SampleColor * max(dot(LightDir, Normal), 0);
	//specular
	vec3 HalfDir = normalize(LightDir + ViewDir);
	vec3 Specular = SampleColor * pow(max(dot(HalfDir, Normal), 0), 32);


	float Shadow = CalcShadow(fs_in.FragPos);
	vec3 TmpColor = Ambient + (1 - Shadow) * (Diffuse + Specular);
	//gammaÐ£Õý
	TmpColor = pow(TmpColor, vec3(1 / 2.2));
	FragColor = vec4(TmpColor, 1);
	//if(Shadow>1)
	//	FragColor = vec4(1, 0, 0, 1);
	//else
	//	FragColor = vec4(Shadow, Shadow, Shadow, 1);

}