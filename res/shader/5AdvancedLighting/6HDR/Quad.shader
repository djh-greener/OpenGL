#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
	gl_Position = vec4(aPos, 1);
	TexCoord = aTexCoord;
}

#shader fragment
#version 420 core

in vec2 TexCoord;

uniform sampler2D HDRTexture;
uniform int Algorithm;
uniform float exposure;

out vec4 FragColor;


void main() {
	vec3 Color = texture(HDRTexture, TexCoord).rgb;
	vec3 TmpColor = 0 * Color;
	if (Algorithm == 0)
	{
		TmpColor = Color;
	}
	else if(Algorithm == 1)
	{
		TmpColor = Color / (Color + vec3(1.0));
	}
	else if (Algorithm == 2)
	{
		TmpColor = vec3(1.0) - exp(-Color * exposure);
	}

	FragColor = vec4(pow(TmpColor, vec3(1 / 2.2)), 1);
}
