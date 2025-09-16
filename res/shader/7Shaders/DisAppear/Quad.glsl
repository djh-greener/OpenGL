#shader vertex
#version 420 core
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(Pos,0, 1.0);
}

#shader fragment
#version 420 core
in vec2 TexCoord;
uniform sampler2D RGBA;

out vec4 FragColor;
void main()
{
	// 从G缓冲中获取数据
	vec3 color = texture(RGBA, TexCoord).rgb;
	float alpha = texture(RGBA, TexCoord).a;
	FragColor = vec4(color/alpha, 1);
}
