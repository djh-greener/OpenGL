#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;
void main() {
	gl_Position = projection * view * model * vec4(aPos, 1);
	vs_out.FragPos = vec3(model * vec4(aPos, 1));
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;
}

#shader fragment
#version 420 core

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;

uniform vec3 LightPos;
uniform sampler2D FloorTexture;
uniform vec3 ViewPos;
uniform bool gamma;
out vec4 FragColor;

void main() {
	vec3 LightDir = normalize(LightPos - fs_in.FragPos);
	vec3 ViewDir = normalize(ViewPos - fs_in.FragPos);
	vec3 Normal = normalize(fs_in.Normal);
	vec3 SampleColor = texture(FloorTexture, fs_in.TexCoord).rgb;
	vec3 Ambient = SampleColor * 0.05;

	//diffuse
	vec3 Diffuse = SampleColor * max(dot(LightDir, Normal), 0);
	//specular
	vec3 HalfDir = normalize(LightDir + ViewDir);
	vec3 Specular = SampleColor*pow(max(dot(HalfDir, Normal),0), 32);
	//Ë¥¼õ
	
	float distance = length(LightPos - fs_in.FragPos);
	float attenuation = 1.0 / (gamma ? distance * distance : distance);
	vec3 TmpColor = (Ambient+Diffuse + Specular) * attenuation;
	//gammaÐ£Õý
	TmpColor = pow(TmpColor, vec3(1/(gamma?2.2:1.0)));
	FragColor = vec4(TmpColor,1);
	//FragColor = vec4(vec3(gamma), 1);


}