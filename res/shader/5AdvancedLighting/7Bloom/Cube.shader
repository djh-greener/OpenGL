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
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;
in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;

struct PointLight {
	vec3 Position;
	vec3 Color;
};

uniform sampler2D Diffuse;
uniform PointLight Lights[4];
uniform vec3 ViewPos;

void main(){
	vec3 ViewDir = normalize(ViewPos - fs_in.FragPos);
	vec3 Normal = normalize(fs_in.Normal);
	vec3 Color = texture(Diffuse, fs_in.TexCoord).rgb;

	vec3 Lighting = 0.05 * Color;
	for (int i = 0; i < 4; i++) {
		vec3 LightDir = normalize(Lights[i].Position - fs_in.FragPos);
		float Diffuse = max(dot(LightDir, Normal), 0);
		float distance = length(Lights[i].Position - fs_in.FragPos);
		//specular
		vec3 HalfDir = normalize(LightDir + ViewDir);
		float Specular = pow(max(dot(HalfDir, Normal), 0), 32);
		vec3 TmpColor = (Diffuse + Specular) * Color * Lights[i].Color;
		Lighting += TmpColor /(distance*distance);
	}
	FragColor = vec4(Lighting, 1);


	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, 1.0);
}
