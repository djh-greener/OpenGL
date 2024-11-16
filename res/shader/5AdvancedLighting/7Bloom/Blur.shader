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

uniform sampler2D Diffuse;
uniform bool horizontal;
float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

out vec4 FragColor;

void main()
{
	vec2 TexOffset = 1.0 / textureSize(Diffuse, 0);
	vec3 result = texture(Diffuse, TexCoord).rgb * weight[0];
	if (horizontal)
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(Diffuse, TexCoord + vec2(TexOffset.x * i, 0.0)).rgb * weight[i];
			result += texture(Diffuse, TexCoord - vec2(TexOffset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(Diffuse, TexCoord + vec2(0.0, TexOffset.y * i)).rgb * weight[i];
			result += texture(Diffuse, TexCoord - vec2(0.0, TexOffset.y * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0);
}
